#ifndef PACIENTE_H
#define PACIENTE_H

#define MAX_NOME 100
#define MAX_CPF 15
#define MAX_DATA 11

/**
 * Estrutura que representa um paciente
 */
typedef struct {
    int id;
    char cpf[MAX_CPF];
    char nome[MAX_NOME];
    int idade;
    char data_cadastro[MAX_DATA];
} Paciente;

/**
 * Imprime os dados de um paciente formatado
 * @param p Ponteiro para o paciente
 */
void paciente_imprimir(const Paciente *p);

/**
 * Imprime o cabeçalho da tabela de pacientes
 */
void paciente_imprimir_cabecalho();

/**
 * Verifica se o nome do paciente começa com o prefixo dado
 * @param p Ponteiro para o paciente
 * @param prefixo String de prefixo a ser verificada
 * @return 1 se encontrou, 0 caso contrário
 */
int paciente_nome_comeca_com(const Paciente *p, const char *prefixo);

/**
 * Verifica se o CPF do paciente começa com o prefixo dado
 * @param p Ponteiro para o paciente
 * @param prefixo String de prefixo a ser verificada
 * @return 1 se encontrou, 0 caso contrário
 */
int paciente_cpf_comeca_com(const Paciente *p, const char *prefixo);
#endif