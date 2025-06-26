#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bd_paciente.h"

BDPaciente* bd_paciente_create() {
    BDPaciente *bd = (BDPaciente*)malloc(sizeof(BDPaciente)); // alocação de memoria para a estrutura
    if (bd == NULL) {
        printf("Erro: Não foi possível alocar memória para o banco de dados.\n");
        return NULL;
    }
    
    bd->pacientes = (Paciente*)malloc(MAX_PACIENTES * sizeof(Paciente)); // alocação de memória para o vetor de pacientes da estrutura
    if (bd->pacientes == NULL) {
        printf("Erro: Não foi possível alocar memória para os pacientes.\n");
        free(bd);
        return NULL;
    }
    
    bd->total = 0;
    bd->capacidade = MAX_PACIENTES;
    
    return bd;
}

// função para desalocar a memória gasta pelo bd de pacientes
void bd_paciente_free(BDPaciente *bd) {
    if (bd != NULL) {
        if (bd->pacientes != NULL) {
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
    
    // remove quebra de linha se existir, e transforma em \n
    char *newline = strchr(linha, '\n');
    if (newline) *newline = '\0';
    
    token = strtok(linha, ","); // divide a string com base nas vírgulas
    while (token != NULL && campo < 5) {
        switch (campo) {
            case 0: // ID
                p->id = atoi(token); // converte string para inteiro
                break;
            case 1: // CPF
                strncpy(p->cpf, token, MAX_CPF - 1);
                p->cpf[MAX_CPF - 1] = '\0';
                break;
            case 2: // Nome
                strncpy(p->nome, token, MAX_NOME - 1);
                p->nome[MAX_NOME - 1] = '\0';
                break;
            case 3: // Idade
                p->idade = atoi(token); // converte string para inteiro
                break;
            case 4: // Data_Cadastro
                strncpy(p->data_cadastro, token, MAX_DATA - 1);
                p->data_cadastro[MAX_DATA - 1] = '\0';
                break;
        }
        campo++;
        token = strtok(NULL, ","); // ao passar NULL no primeiro parametro, ele vai continuar de onde parou, retornando ponteiro para o proximo token ou NULL quando acabar
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
        
        // cria uma cópia da linha para não modificar a original
        char linha_copia[512];
        strcpy(linha_copia, linha);
        
        // faz o parsing da linha. se for bem sucedido, conta mais uma linha no total do bd
        if (parsear_linha_csv(linha_copia, &bd->pacientes[bd->total])) {
            bd->total++;
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
        if (paciente_nome_comeca_com(&bd->pacientes[i], nome_prefixo)) {
            if (encontrados == 0) {
                paciente_imprimir_cabecalho();
            }
            paciente_imprimir(&bd->pacientes[i]);
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
        if (paciente_cpf_comeca_com(&bd->pacientes[i], cpf_prefixo)) {
            if (encontrados == 0) {
                paciente_imprimir_cabecalho();
            }
            paciente_imprimir(&bd->pacientes[i]);
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
        paciente_imprimir(&bd->pacientes[i]);
    }
    
    printf("\nTotal: %d paciente(s).\n", bd->total);
}

int bd_paciente_total(const BDPaciente *bd) {
    return (bd != NULL) ? bd->total : 0;
}