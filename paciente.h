#ifndef PACIENTE_H
#define PACIENTE_H

#define MAX_NOME 100
#define MAX_CPF 15
#define MAX_DATA 11

/**
 * Estrutura que representa um paciente (tipo opaco)
 */
typedef struct paciente Paciente;

/**
 * Cria um novo paciente
 * @return Ponteiro para o paciente criado, ou NULL em caso de erro
 */
Paciente* paciente_create();

/**
 * Destroi um paciente e libera a memória
 * @param p Ponteiro para o paciente
 */
void paciente_free(Paciente *p);

/**
 * Funções de acesso (getters)
 */
int paciente_get_id(const Paciente *p);
const char* paciente_get_cpf(const Paciente *p);
const char* paciente_get_nome(const Paciente *p);
int paciente_get_idade(const Paciente *p);
const char* paciente_get_data_cadastro(const Paciente *p);

/**
 * Funções de modificação (setters)
 */
void paciente_set_id(Paciente *p, int id);
void paciente_set_cpf(Paciente *p, const char *cpf);
void paciente_set_nome(Paciente *p, const char *nome);
void paciente_set_idade(Paciente *p, int idade);
void paciente_set_data_cadastro(Paciente *p, const char *data_cadastro);

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