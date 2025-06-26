#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "paciente.h"

struct paciente {
    int id;
    char cpf[MAX_CPF];
    char nome[MAX_NOME];
    int idade;
    char data_cadastro[MAX_DATA];
};

/**
 * Função para converter o nome para minúsculo
 * @param original Ponteiro para o nome original
 * @param destino Ponteiro para a string destino do nome minusculo
 */
static void converter_nome_minusculo(const char* original, char* destino){
    // copia e converte string para minúscula
    int i = 0;
    while (original[i] && i < MAX_NOME - 1) {
        destino[i] = tolower(original[i]);
        i++;
    }
    destino[i] = '\0';
}

/**
 * Função para converter o cpf, removendo pontos, hífens e espaços vazios
 * @param original Ponteiro para o nome original
 * @param destino Ponteiro para a string destino do nome minusculo
 */
static void converter_cpf_apenas_numeros(const char* original, char* destino){
    // copia e converte string para ficar sem pontos, hifens e espaços em branco
    int i = 0, j = 0;
    while (original[i] && i < MAX_CPF - 1){
        if (original[i] != '-' && original[i] != '.' && original[i] != ' '){
            destino[j] = original[i];
            j++;
        }
        i++;
    }
    destino[j] = '\0';
}

Paciente* paciente_create() {
    Paciente *p = (Paciente*)malloc(sizeof(Paciente));
    if (p == NULL) {
        return NULL;
    }
    
    // Inicializa com valores padrão
    p->id = 0;
    p->cpf[0] = '\0';
    p->nome[0] = '\0';
    p->idade = 0;
    p->data_cadastro[0] = '\0';
    
    return p;
}

void paciente_free(Paciente *p) {
    if (p != NULL) {
        free(p);
    }
}

// Funções de acesso (getters)
int paciente_get_id(const Paciente *p) {
    return (p != NULL) ? p->id : 0;
}

const char* paciente_get_cpf(const Paciente *p) {
    return (p != NULL) ? p->cpf : "";
}

const char* paciente_get_nome(const Paciente *p) {
    return (p != NULL) ? p->nome : "";
}

int paciente_get_idade(const Paciente *p) {
    return (p != NULL) ? p->idade : 0;
}

const char* paciente_get_data_cadastro(const Paciente *p) {
    return (p != NULL) ? p->data_cadastro : "";
}

// Funções de modificação (setters)
void paciente_set_id(Paciente *p, int id) {
    if (p != NULL) {
        p->id = id;
    }
}

void paciente_set_cpf(Paciente *p, const char *cpf) {
    if (p != NULL && cpf != NULL) {
        strncpy(p->cpf, cpf, MAX_CPF - 1);
        p->cpf[MAX_CPF - 1] = '\0';
    }
}

void paciente_set_nome(Paciente *p, const char *nome) {
    if (p != NULL && nome != NULL) {
        strncpy(p->nome, nome, MAX_NOME - 1);
        p->nome[MAX_NOME - 1] = '\0';
    }
}

void paciente_set_idade(Paciente *p, int idade) {
    if (p != NULL) {
        p->idade = idade;
    }
}

void paciente_set_data_cadastro(Paciente *p, const char *data_cadastro) {
    if (p != NULL && data_cadastro != NULL) {
        strncpy(p->data_cadastro, data_cadastro, MAX_DATA - 1);
        p->data_cadastro[MAX_DATA - 1] = '\0';
    }
}

void paciente_imprimir_cabecalho() {
    printf("%-3s %-15s %-20s %-5s %-12s\n", "ID", "CPF", "Nome", "Idade", "Data_Cadastro");
    printf("%-3s %-15s %-20s %-5s %-12s\n", "---", "---------------", "--------------------", "-----", "------------");
}

void paciente_imprimir(const Paciente *p) {
    if (p == NULL) return;
    printf("%-3d %-15s %-20s %-5d %-12s\n", 
           p->id, p->cpf, p->nome, p->idade, p->data_cadastro);
}

int paciente_nome_comeca_com(const Paciente *p, const char *prefixo) {
    if (p == NULL || prefixo == NULL) return 0;
    
    // converte ambos para minúscula para comparação case-insensitive
    char nome_lower[MAX_NOME];
    char prefixo_lower[MAX_NOME];
    
    // copia e converte nome e prefixo para minúscula
    converter_nome_minusculo(p->nome, nome_lower);
    converter_nome_minusculo(prefixo, prefixo_lower);

    // verifica se o nome começa com o prefixo
    return strncmp(nome_lower, prefixo_lower, strlen(prefixo_lower)) == 0;
}

int paciente_cpf_comeca_com(const Paciente *p, const char *prefixo) {
    if (p == NULL || prefixo == NULL) return 0;

    // remover pontos e hífens para não atrapalhar na busca
    char cpf_limpo[MAX_CPF];
    char prefixo_limpo[MAX_CPF];

    converter_cpf_apenas_numeros(p->cpf, cpf_limpo);
    converter_cpf_apenas_numeros(prefixo, prefixo_limpo);
    
    return strncmp(cpf_limpo, prefixo_limpo, strlen(prefixo_limpo)) == 0;
}