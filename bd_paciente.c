#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bd_paciente.h"
#include "paciente.h"

/**
 * Estrutura que representa o banco de dados de pacientes
 */
struct bdpaciente{
    Paciente **pacientes;  // Vetor de ponteiros para pacientes
    int total;            // Número atual de pacientes
    int capacidade;       // Capacidade máxima
};

BDPaciente* bd_paciente_create() {
    BDPaciente *bd = (BDPaciente*)malloc(sizeof(BDPaciente));
    if (bd == NULL) {
        printf("Erro: Não foi possível alocar memória para o banco de dados.\n");
        return NULL;
    }
    
    bd->pacientes = (Paciente**)malloc(MAX_PACIENTES * sizeof(Paciente*));
    if (bd->pacientes == NULL) {
        printf("Erro: Não foi possível alocar memória para os pacientes.\n");
        free(bd);
        return NULL;
    }
    
    // Inicializa todos os ponteiros como NULL
    for (int i = 0; i < MAX_PACIENTES; i++) {
        bd->pacientes[i] = NULL;
    }
    
    bd->total = 0;
    bd->capacidade = MAX_PACIENTES;
    
    return bd;
}

void bd_paciente_free(BDPaciente *bd) {
    if (bd != NULL) {
        if (bd->pacientes != NULL) {
            // Libera cada paciente individualmente
            for (int i = 0; i < bd->total; i++) {
                if (bd->pacientes[i] != NULL) {
                    paciente_free(bd->pacientes[i]);
                }
            }
            free(bd->pacientes);
        }
        free(bd);
    }
}

/**
 * função auxiliar para fazer o parsing de uma linha CSV
 */
static int parsear_linha_csv(char *linha, Paciente *p) {
    char *token;
    int campo = 0;
    
    // remove quebra de linha se existir
    char *newline = strchr(linha, '\n');
    if (newline) *newline = '\0';
    
    token = strtok(linha, ",");
    while (token != NULL && campo < 5) {
        switch (campo) {
            case 0: // ID
                paciente_set_id(p, atoi(token));
                break;
            case 1: // CPF
                paciente_set_cpf(p, token);
                break;
            case 2: // Nome
                paciente_set_nome(p, token);
                break;
            case 3: // Idade
                paciente_set_idade(p, atoi(token));
                break;
            case 4: // Data_Cadastro
                paciente_set_data_cadastro(p, token);
                break;
        }
        campo++;
        token = strtok(NULL, ",");
    }
    
    return campo == 5; // retorna 1 se todos os 5 campos foram lidos
}

int bd_paciente_carregar_arquivo(BDPaciente *bd, const char *nome_arquivo) {
    if (bd == NULL || nome_arquivo == NULL) {
        return 0;
    }
    
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro: Não foi possível abrir o arquivo %s.\n", nome_arquivo);
        return 0;
    }
    
    char linha[512];
    int primeira_linha = 1;
    
    // lê linha por linha
    while (fgets(linha, sizeof(linha), arquivo) && bd->total < bd->capacidade) {
        // pula a primeira linha (cabeçalho)
        if (primeira_linha) {
            primeira_linha = 0;
            continue;
        }
        
        // cria um novo paciente
        Paciente *novo_paciente = paciente_create();
        if (novo_paciente == NULL) {
            printf("Erro: Não foi possível criar paciente.\n");
            continue;
        }
        
        // cria uma cópia da linha para não modificar a original
        char linha_copia[512];
        strcpy(linha_copia, linha);
        
        // faz o parsing da linha
        if (parsear_linha_csv(linha_copia, novo_paciente)) {
            bd->pacientes[bd->total] = novo_paciente;
            bd->total++;
        } else {
            // Se falhou no parsing, libera o paciente criado
            paciente_free(novo_paciente);
        }
    }
    
    fclose(arquivo);
    printf("Banco de dados carregado com sucesso! %d pacientes encontrados.\n", bd->total);
    return 1;
}

void bd_paciente_consultar_por_nome(const BDPaciente *bd, const char *nome_prefixo) {
    if (bd == NULL || nome_prefixo == NULL) {
        return;
    }
    
    int encontrados = 0;
    
    for (int i = 0; i < bd->total; i++) {
        if (paciente_nome_comeca_com(bd->pacientes[i], nome_prefixo)) {
            if (encontrados == 0) {
                paciente_imprimir_cabecalho();
            }
            paciente_imprimir(bd->pacientes[i]);
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        printf("Nenhum paciente encontrado com o nome que começa com '%s'.\n", nome_prefixo);
    } else {
        printf("\n%d paciente(s) encontrado(s).\n", encontrados);
    }
}

void bd_paciente_consultar_por_cpf(const BDPaciente *bd, const char *cpf_prefixo) {
    if (bd == NULL || cpf_prefixo == NULL) {
        return;
    }
    
    int encontrados = 0;
    
    for (int i = 0; i < bd->total; i++) {
        if (paciente_cpf_comeca_com(bd->pacientes[i], cpf_prefixo)) {
            if (encontrados == 0) {
                paciente_imprimir_cabecalho();
            }
            paciente_imprimir(bd->pacientes[i]);
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        printf("Nenhum paciente encontrado com CPF que começa com '%s'.\n", cpf_prefixo);
    } else {
        printf("\n%d paciente(s) encontrado(s).\n", encontrados);
    }
}

void bd_paciente_listar_todos(const BDPaciente *bd) {
    if (bd == NULL) {
        return;
    }
    
    if (bd->total == 0) {
        printf("Nenhum paciente cadastrado no sistema.\n");
        return;
    }
    
    printf("Imprimindo lista de pacientes...\n");
    paciente_imprimir_cabecalho();
    
    for (int i = 0; i < bd->total; i++) {
        paciente_imprimir(bd->pacientes[i]);
    }
    
    printf("\nTotal: %d paciente(s).\n", bd->total);
}

int bd_paciente_total(const BDPaciente *bd) {
    return (bd != NULL) ? bd->total : 0;
}