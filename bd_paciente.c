#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bd_paciente.h"
#include "paciente.h"

/**
 * Estrutura do nó da linked list
 */
typedef struct no_paciente {
    Paciente *paciente;
    struct no_paciente *proximo;
} NoPaciente;

/**
 * Estrutura que representa o banco de dados de pacientes (linked list de pacientes)
 */
struct bdpaciente {
    NoPaciente *primeiro;  // Primeiro nó da lista
    int total;             // Contador de pacientes
};

BDPaciente* bd_paciente_create() {
    BDPaciente *bd = (BDPaciente*)malloc(sizeof(BDPaciente));
    if (bd == NULL) {
        printf("Erro: Não foi possível alocar memória para o banco de dados.\n");
        return NULL;
    }
    
    bd->primeiro = NULL;
    bd->total = 0;
    
    return bd;
}

void bd_paciente_free(BDPaciente *bd) {
    if (bd == NULL) {
        return;
    }
    
    NoPaciente *atual = bd->primeiro;
    while (atual != NULL) {
        NoPaciente *proximo = atual->proximo;
        paciente_free(atual->paciente);
        free(atual);
        atual = proximo;
    }
    
    free(bd);
}

/**
 * Função auxiliar para inserir paciente no final da lista
 */
int bd_paciente_inserir(BDPaciente *bd, Paciente *p) {
    if (bd == NULL || p == NULL) {
        return 0;
    }
    
    NoPaciente *novo_no = (NoPaciente*)malloc(sizeof(NoPaciente));
    if (novo_no == NULL) {
        return 0;
    }
    
    novo_no->paciente = p;
    novo_no->proximo = NULL;
    
    // Se a lista está vazia
    if (bd->primeiro == NULL) {
        bd->primeiro = novo_no;
    } else {
        // Encontra o último nó
        NoPaciente *atual = bd->primeiro;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novo_no;
    }
    
    bd->total++;
    return 1;
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
    int pacientes_carregados = 0;
    
    // lê linha por linha
    while (fgets(linha, sizeof(linha), arquivo)) {
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
            if (bd_paciente_inserir(bd, novo_paciente)) {
                pacientes_carregados++;
            } else {
                paciente_free(novo_paciente);
            }
        } else {
            // Se falhou no parsing, libera o paciente criado
            paciente_free(novo_paciente);
        }
    }
    
    fclose(arquivo);
    printf("Banco de dados carregado com sucesso! %d pacientes encontrados.\n", pacientes_carregados);
    return 1;
}

void bd_paciente_consultar_por_nome(const BDPaciente *bd, const char *nome_prefixo) {
    if (bd == NULL || nome_prefixo == NULL) {
        return;
    }
    
    printf("Buscando pacientes com nome que começa com '%s'...\n", nome_prefixo);
    
    int contador = 0;
    int primeira_impressao = 1;
    
    // Itera pela lista ligada
    NoPaciente *atual = bd->primeiro;
    while (atual != NULL) {
        if (paciente_nome_comeca_com(atual->paciente, nome_prefixo)) {
            if (primeira_impressao) {
                paciente_imprimir_cabecalho();
                primeira_impressao = 0;
            }
            paciente_imprimir(atual->paciente);
            contador++;
        }
        atual = atual->proximo;
    }
    
    if (contador == 0) {
        printf("Nenhum paciente encontrado com o nome que começa com '%s'.\n", nome_prefixo);
    } else {
        printf("\n%d paciente(s) encontrado(s).\n", contador);
    }
}

void bd_paciente_consultar_por_cpf(const BDPaciente *bd, const char *cpf_prefixo) {
    if (bd == NULL || cpf_prefixo == NULL) {
        return;
    }
    
    printf("Buscando pacientes com CPF que começa com '%s'...\n", cpf_prefixo);
    
    int contador = 0;
    int primeira_impressao = 1;
    
    NoPaciente *atual = bd->primeiro;
    while (atual != NULL) {
        if (paciente_cpf_comeca_com(atual->paciente, cpf_prefixo)) {
            if (primeira_impressao) {
                paciente_imprimir_cabecalho();
                primeira_impressao = 0;
            }
            paciente_imprimir(atual->paciente);
            contador++;
        }
        atual = atual->proximo;
    }
    
    if (contador == 0) {
        printf("Nenhum paciente encontrado com CPF que começa com '%s'.\n", cpf_prefixo);
    } else {
        printf("\n%d paciente(s) encontrado(s).\n", contador);
    }
}

void bd_paciente_listar_todos(const BDPaciente *bd) {
    if (bd == NULL) {
        return;
    }
    
    if (bd->primeiro == NULL) {
        printf("Nenhum paciente cadastrado no sistema.\n");
        return;
    }
    
    printf("Imprimindo lista de pacientes...\n");
    paciente_imprimir_cabecalho();
    
    NoPaciente *atual = bd->primeiro;
    while (atual != NULL) {
        paciente_imprimir(atual->paciente);
        atual = atual->proximo;
    }
    
    printf("\nTotal: %d paciente(s).\n", bd->total);
}

int bd_paciente_total(const BDPaciente *bd) {
    return (bd != NULL) ? bd->total : 0;
}


// função para remover um paciente por ID
int bd_paciente_remover_por_id(BDPaciente *bd, int id) {
    if (bd == NULL) {
        return 0;
    }
    
    NoPaciente *atual = bd->primeiro;
    NoPaciente *anterior = NULL;
    
    while (atual != NULL) {
        if (paciente_get_id(atual->paciente) == id) {
            // remove o nó da lista
            if (anterior == NULL) {
                // é o primeiro nó
                bd->primeiro = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            
            // libera memória
            paciente_free(atual->paciente);
            free(atual);
            bd->total--;
            return 1;
        }
        
        anterior = atual;
        atual = atual->proximo;
    }
    
    return 0; // paciente não encontrado
}

// Função para buscar um paciente por ID
Paciente* bd_paciente_buscar_por_id(const BDPaciente *bd, int id) {
    if (bd == NULL) {
        return NULL;
    }
    
    NoPaciente *atual = bd->primeiro;
    while (atual != NULL) {
        if (paciente_get_id(atual->paciente) == id) {
            return atual->paciente;
        }
        atual = atual->proximo;
    }
    
    return NULL;
}

// função para encontrar o maior ID existente
int bd_paciente_maior_id(const BDPaciente *bd) {
    if (bd == NULL || bd->primeiro == NULL) {
        return 0;
    }
    
    int maior_id = 0;
    NoPaciente *atual = bd->primeiro;
    
    while (atual != NULL) {
        int id_atual = paciente_get_id(atual->paciente);
        if (id_atual > maior_id) {
            maior_id = id_atual;
        }
        atual = atual->proximo;
    }
    
    return maior_id;
}

// função para salvar dados no arquivo CSV
int bd_paciente_salvar_arquivo(const BDPaciente *bd, const char *nome_arquivo) {
    if (bd == NULL || nome_arquivo == NULL) {
        return 0;
    }
    
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro: Não foi possível criar/abrir o arquivo %s para escrita.\n", nome_arquivo);
        return 0;
    }
    
    // cabeçalho
    fprintf(arquivo, "ID,CPF,Nome,Idade,Data_Cadastro\n");
    
    // dados dos pacientes
    NoPaciente *atual = bd->primeiro;
    while (atual != NULL) {
        fprintf(arquivo, "%d,%s,%s,%d,%s\n",
                paciente_get_id(atual->paciente),
                paciente_get_cpf(atual->paciente),
                paciente_get_nome(atual->paciente),
                paciente_get_idade(atual->paciente),
                paciente_get_data_cadastro(atual->paciente));
        atual = atual->proximo;
    }
    
    fclose(arquivo);
    printf("Dados salvos no arquivo %s com sucesso!\n", nome_arquivo);
    return 1;
}