#ifndef BD_PACIENTE_H
#define BD_PACIENTE_H

#include "paciente.h"

#define MAX_PACIENTES 20
#define NOME_ARQUIVO "bd_paciente.csv"

/**
 * Estrutura que representa o banco de dados de pacientes
 */
typedef struct {
    Paciente *pacientes;  // Vetor de pacientes
    int total;           // Número atual de pacientes
    int capacidade;      // Capacidade máxima
} BDPaciente;

/**
 * Cria e inicializa um novo banco de dados de pacientes
 * @return Ponteiro para o BDPaciente criado, ou NULL em caso de erro
 */
BDPaciente* bd_paciente_create();

/**
 * Destroi o banco de dados e libera a memória
 * @param bd Ponteiro para o banco de dados
 */
void bd_paciente_free(BDPaciente *bd);

/**
 * Carrega os dados do arquivo CSV para o banco de dados
 * @param bd Ponteiro para o banco de dados
 * @param nome_arquivo Nome do arquivo CSV
 * @return 1 se sucesso, 0 se erro
 */
int bd_paciente_carregar_arquivo(BDPaciente *bd, const char *nome_arquivo);

/**
 * Consulta pacientes por nome (busca por prefixo)
 * @param bd Ponteiro para o banco de dados
 * @param nome_prefixo Prefixo do nome a ser buscado
 */
void bd_paciente_consultar_por_nome(const BDPaciente *bd, const char *nome_prefixo);

/**
 * Consulta pacientes por CPF (busca por prefixo)
 * @param bd Ponteiro para o banco de dados
 * @param cpf_prefixo Prefixo do CPF a ser buscado
 */
void bd_paciente_consultar_por_cpf(const BDPaciente *bd, const char *cpf_prefixo);

/**
 * Lista todos os pacientes do banco de dados
 * @param bd Ponteiro para o banco de dados
 */
void bd_paciente_listar_todos(const BDPaciente *bd);

/**
 * Retorna o número total de pacientes no banco
 * @param bd Ponteiro para o banco de dados
 * @return Número de pacientes
 */
int bd_paciente_total(const BDPaciente *bd);

#endif