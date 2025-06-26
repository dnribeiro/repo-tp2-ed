#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bd_paciente.h"

#define MAX_INPUT 256
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
    printf("2 - Imprimir lista de pacientes\n");
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
    bd_paciente_free(bd); // vai que chega no final do programa sem nenhum free rodar kkkkkkkk
    return 0;
}