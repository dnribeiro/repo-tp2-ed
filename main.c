#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bd_paciente.h"
#include <time.h>

#define MAX_INPUT 256

// função para obter a data atual no formato yyyy-mm-dd
void obter_data_atual(char *buffer, size_t tamanho) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, tamanho, "%Y-%m-%d", tm_info);
}

void salvar_alteracoes(BDPaciente *bd) {
    if (bd_paciente_salvar_arquivo(bd, NOME_ARQUIVO)) {
        printf("Alterações salvas automaticamente no arquivo.\n");
    } else {
        printf("Aviso: Não foi possível salvar as alterações no arquivo.\n");
    }
}

// função pra limpar o \n deixado pelo enter do ultimo scanf
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// função pra tornar o fgets melhor de usar, evitando buffer overflow na leitura dos prefixos
void ler_string(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin)) {
        // remove a quebra de linha se existir
        char *newline = strchr(buffer, '\n');
        if (newline) *newline = '\0';
    }
}

void exibir_menu_principal() {
    printf("\n=== Sistema de Gerenciamento de Pacientes ===\n");
    printf("1 - Consultar paciente\n");
    printf("2 - Atualizar paciente\n");
    printf("3 - Remover paciente\n");
    printf("4 - Inserir paciente\n");
    printf("5 - Imprimir lista de pacientes\n");
    printf("Q - Sair\n");
    printf("Escolha uma opção: ");
}

void exibir_menu_consulta() {
    printf("\nEscolha o modo de consulta:\n");
    printf("1 - Por nome\n");
    printf("2 - Por CPF\n");
    printf("3 - Retornar ao menu principal\n");
    printf("Escolha uma opção: ");
}

// Nova função que realiza uma única consulta sem loop
int realizar_consulta_simples(BDPaciente *bd) {
    char opcao;
    char input[MAX_INPUT];
    
    exibir_menu_consulta();
    scanf(" %c", &opcao);
    limpar_buffer();
    
    switch (opcao) {
        case '1':
            printf("Digite o nome: ");
            ler_string(input, MAX_INPUT);
            printf("\n");
            bd_paciente_consultar_por_nome(bd, input);
            return 1; // consulta realizada
            
        case '2':
            printf("Digite o CPF: ");
            ler_string(input, MAX_INPUT);
            printf("\n");
            bd_paciente_consultar_por_cpf(bd, input);
            return 1; // consulta realizada
            
        case '3':
            return 0; // cancelar operação
            
        default:
            printf("Opção inválida! Tente novamente.\n");
            return -1; // opção inválida, tentar novamente
    }
}

void executar_consulta(BDPaciente *bd) {
    char opcao;
    char input[MAX_INPUT];
    
    while (1) {
        exibir_menu_consulta();
        scanf(" %c", &opcao);
        limpar_buffer();
        
        switch (opcao) {
            case '1':
                printf("Digite o nome: ");
                ler_string(input, MAX_INPUT);
                printf("\n");
                bd_paciente_consultar_por_nome(bd, input);
                break;
                
            case '2':
                printf("Digite o CPF: ");
                ler_string(input, MAX_INPUT);
                printf("\n");
                bd_paciente_consultar_por_cpf(bd, input);
                break;
                
            case '3':
                return; // retornar ao programa principal
                
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
        
        printf("\nPressione Enter para continuar...");
        getchar(); // serve pra "pausar" o programa até o usuário digitar enter
    }
}

// Função para validar CPF (apenas dígitos)
int validar_cpf(const char *cpf) {
    if (strlen(cpf) != 11) return 0;
    
    for (int i = 0; i < 11; i++) {
        if (cpf[i] < '0' || cpf[i] > '9') return 0;
    }
    return 1;
}

// função para formatar CPF (adicionar pontos e hífen)
void formatar_cpf(const char *cpf_numeros, char *cpf_formatado) {
    sprintf(cpf_formatado, "%.3s.%.3s.%.3s-%.2s", 
            cpf_numeros, cpf_numeros+3, cpf_numeros+6, cpf_numeros+9);
}

int gerar_proximo_id(BDPaciente *bd) {
    return bd_paciente_maior_id(bd) + 1;
}

void executar_atualizacao(BDPaciente *bd) {
    printf("Faça uma consulta para localizar o paciente que deseja atualizar:\n");
    
    // permite múltiplas consultas até encontrar o paciente desejado
    while (1) {
        int resultado = realizar_consulta_simples(bd);
        
        if (resultado == 0) {
            printf("Operação de atualização cancelada.\n");
            return;
        } else if (resultado == -1) {
            // opção inválida
            continue;
        } else {
            // consulta realizada com sucesso
            break;
        }
    }
    
    int id;
    printf("\nDigite o ID do registro a ser atualizado: ");
    scanf("%d", &id);
    limpar_buffer();
    
    Paciente *paciente = bd_paciente_buscar_por_id(bd, id);
    if (paciente == NULL) {
        printf("Paciente com ID %d não encontrado.\n", id);
        return;
    }
    
    printf("\nRegistro atual:\n");
    paciente_imprimir_cabecalho();
    paciente_imprimir(paciente);
    
    printf("\nDigite o novo valor para os campos CPF (apenas dígitos), Nome e sIdade\n");
    printf("(para manter o valor atual de um campo, digite '-'):\n");
    
    char cpf_input[MAX_INPUT];
    char nome_input[MAX_INPUT];
    char idade_input[MAX_INPUT];
    
    printf("CPF: ");
    ler_string(cpf_input, MAX_INPUT);
    
    printf("Nome: ");
    ler_string(nome_input, MAX_INPUT);
    
    printf("Idade: ");
    ler_string(idade_input, MAX_INPUT);
    
    // criar paciente temporário com os novos valores
    Paciente *temp = paciente_create();
    if (temp == NULL) {
        printf("Erro ao criar paciente temporário.\n");
        return;
    }
    paciente_set_id(temp, paciente_get_id(paciente));
    paciente_set_cpf(temp, paciente_get_cpf(paciente));
    paciente_set_nome(temp, paciente_get_nome(paciente));
    paciente_set_idade(temp, paciente_get_idade(paciente));
    paciente_set_data_cadastro(temp, paciente_get_data_cadastro(paciente));
    
    // atualizar campos modificados
    if (strcmp(cpf_input, "-") != 0) {
        if (validar_cpf(cpf_input)) {
            char cpf_formatado[MAX_INPUT];
            formatar_cpf(cpf_input, cpf_formatado);
            paciente_set_cpf(temp, cpf_formatado);
        } else {
            printf("CPF inválido! Deve conter 11 dígitos.\n");
            paciente_free(temp);
            return;
        }
    }
    
    if (strcmp(nome_input, "-") != 0) {
        paciente_set_nome(temp, nome_input);
    }
    
    if (strcmp(idade_input, "-") != 0) {
        int idade = atoi(idade_input);
        if (idade > 0) {
            paciente_set_idade(temp, idade);
        } else {
            printf("Idade inválida!\n");
            paciente_free(temp);
            return;
        }
    }
    
    // mostrar preview dos novos valores
    printf("\nConfirma os novos valores para o registro abaixo? (S/N)\n");
    paciente_imprimir_cabecalho();
    paciente_imprimir(temp);
    
    char confirmacao;
    printf("Resposta: ");
    scanf(" %c", &confirmacao);
    limpar_buffer();
    
    if (confirmacao == 'S' || confirmacao == 's') {
        // atualizar o paciente original
        paciente_set_cpf(paciente, paciente_get_cpf(temp));
        paciente_set_nome(paciente, paciente_get_nome(temp));
        paciente_set_idade(paciente, paciente_get_idade(temp));
        paciente_set_data_cadastro(paciente, paciente_get_data_cadastro(temp));

        salvar_alteracoes(bd);
        printf("Registro atualizado com sucesso.\n");
    } else {
        printf("Atualização cancelada.\n");
    }
    
    paciente_free(temp);
}

void executar_remocao(BDPaciente *bd) {
    printf("Faça uma consulta para localizar o paciente que deseja remover:\n");
    
    // permite múltiplas consultas até encontrar o paciente desejado
    while (1) {
        int resultado = realizar_consulta_simples(bd);
        
        if (resultado == 0) {
            printf("Operação de atualização cancelada.\n");
            return;
        } else if (resultado == -1) {
            // opção inválida
            continue;
        } else {
            // consulta realizada com sucesso
            break;
        }
    }
    
    int id;
    printf("\nDigite o ID do registro a ser removido: ");
    scanf("%d", &id);
    limpar_buffer();
    
    Paciente *paciente = bd_paciente_buscar_por_id(bd, id);
    if (paciente == NULL) {
        printf("Paciente com ID %d não encontrado.\n", id);
        return;
    }
    
    printf("\nTem certeza de que deseja excluir o registro abaixo? (S/N)\n");
    paciente_imprimir_cabecalho();
    paciente_imprimir(paciente);
    
    char confirmacao;
    printf("Resposta: ");
    scanf(" %c", &confirmacao);
    limpar_buffer();
    
    if (confirmacao == 'S' || confirmacao == 's') {
        if (bd_paciente_remover_por_id(bd, id)) {
            salvar_alteracoes(bd);
            printf("Registro removido com sucesso.\n");
        } else {
            printf("Erro ao remover o registro.\n");
        }
    } else {
        printf("Remoção cancelada.\n");
    }
}

void executar_insercao(BDPaciente *bd) {
    printf("Para inserir um novo registro, digite os valores para os campos CPF (apenas dígitos), nome e idade.\n A data de cadastro será preenchida automaticamente com a data atual.\n\n");
    
    char cpf_input[MAX_INPUT];
    char nome_input[MAX_INPUT];
    char idade_input[MAX_INPUT];
    char data_atual[MAX_INPUT];
    
    printf("CPF: ");
    ler_string(cpf_input, MAX_INPUT);
    
    if (!validar_cpf(cpf_input)) {
        printf("CPF inválido! Deve conter 11 dígitos.\n");
        return;
    }
    
    printf("Nome: ");
    ler_string(nome_input, MAX_INPUT);
    
    if (strlen(nome_input) == 0) {
        printf("Nome não pode estar vazio.\n");
        return;
    }
    
    printf("Idade: ");
    ler_string(idade_input, MAX_INPUT);
    
    int idade = atoi(idade_input);
    if (idade <= 0) {
        printf("Idade inválida!\n");
        return;
    }
    
    obter_data_atual(data_atual, MAX_INPUT);
    printf("Data de cadastro (automática): %s\n", data_atual);
    
    // cria novo paciente
    Paciente *novo_paciente = paciente_create();
    if (novo_paciente == NULL) {
        printf("Erro ao criar novo paciente.\n");
        return;
    }
    
    // gerar ID automático
    int novo_id = gerar_proximo_id(bd);
    paciente_set_id(novo_paciente, novo_id);
    
    // formatar e definir CPF
    char cpf_formatado[MAX_INPUT];
    formatar_cpf(cpf_input, cpf_formatado);
    paciente_set_cpf(novo_paciente, cpf_formatado);
    
    paciente_set_nome(novo_paciente, nome_input);
    paciente_set_idade(novo_paciente, idade);
    paciente_set_data_cadastro(novo_paciente, data_atual);
    
    // mostrar preview
    printf("\nConfirma a inserção do registro abaixo? (S/N)\n");
    paciente_imprimir_cabecalho();
    paciente_imprimir(novo_paciente);
    
    char confirmacao;
    printf("Resposta: ");
    scanf(" %c", &confirmacao);
    limpar_buffer();
    
    if (confirmacao == 'S' || confirmacao == 's') {
        if (bd_paciente_inserir(bd, novo_paciente)) {
            salvar_alteracoes(bd);
            printf("O registro foi inserido com sucesso.\n");
        } else {
            printf("Erro ao inserir o registro.\n");
            paciente_free(novo_paciente);
        }
    } else {
        printf("Inserção cancelada.\n");
        paciente_free(novo_paciente);
    }
}

int main() {
    BDPaciente *bd = bd_paciente_create();
    if (bd == NULL) {
        printf("Erro: Não foi possível inicializar o sistema.\n");
        return 1;
    }
    
    // carrega os dados do arquivo
    if (!bd_paciente_carregar_arquivo(bd, NOME_ARQUIVO)) {
        printf("Aviso: Sistema iniciado sem dados.\n");
    }
    
    char opcao;
    
    // loop principal do sistema
    while (1) {
        exibir_menu_principal();
        scanf(" %c", &opcao);
        limpar_buffer();
        
        switch (opcao) {
            case '1':
                executar_consulta(bd);
                break;
                
            case '2':
                executar_atualizacao(bd);
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
                
            case '3':
                executar_remocao(bd);
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
                
            case '4':
                executar_insercao(bd);
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
                
            case '5':
                printf("\n");
                bd_paciente_listar_todos(bd);
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
                
            case 'Q':
            case 'q':
                printf("Encerrando o sistema...\n");
                bd_paciente_free(bd);
                return 0;
                
            default:
                printf("Opção inválida! Tente novamente.\n");
                printf("Pressione Enter para continuar...");
                getchar();
                break;
        }
    }
    bd_paciente_free(bd);
    return 0;
}