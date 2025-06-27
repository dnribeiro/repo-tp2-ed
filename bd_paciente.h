#ifndef BD_PACIENTE_H
#define BD_PACIENTE_H

#include "paciente.h"

#define NOME_ARQUIVO "bd_paciente.csv"

/**
 * Estrutura que representa o banco de dados de pacientes
 */
typedef struct bdpaciente BDPaciente;

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

/**
 * Insere um novo paciente no banco de dados
 * @param bd Ponteiro para o banco de dados
 * @param p Ponteiro para o paciente a ser inserido
 * @return 1 se sucesso, 0 se erro
 */
int bd_paciente_inserir(BDPaciente *bd, Paciente *p);

/**
 * Remove um paciente do banco de dados por ID
 * @param bd Ponteiro para o banco de dados
 * @param id ID do paciente a ser removido
 * @return 1 se sucesso, 0 se não encontrado
 */
int bd_paciente_remover_por_id(BDPaciente *bd, int id);

/**
 * Busca um paciente por ID
 * @param bd Ponteiro para o banco de dados
 * @param id ID do paciente a ser buscado
 * @return Ponteiro para o paciente se encontrado, NULL caso contrário
 */
Paciente* bd_paciente_buscar_por_id(const BDPaciente *bd, int id);

/**
 * Encontra o maior ID existente no banco de dados
 * @param bd Ponteiro para o banco de dados
 * @return Maior ID encontrado, ou 0 se não houver registros
 */
int bd_paciente_maior_id(const BDPaciente *bd);

#endif