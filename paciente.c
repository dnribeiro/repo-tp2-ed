#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "paciente.h"

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

void paciente_imprimir_cabecalho() {
    printf("%-3s %-15s %-20s %-5s %-12s\n", "ID", "CPF", "Nome", "Idade", "Data_Cadastro"); // id: 3 espaços, cpf: 15 espaços, nome:20 espaços, idade: 5 espaços, data de cadastro: 12 espaços
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