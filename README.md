
# HealthSys - Sistema de Gerenciamento de Pacientes

## Descrição

HealthSys é um sistema simplificado de gerenciamento de dados de pacientes de uma clínica, desenvolvido em linguagem C. Os dados são armazenados em um arquivo `.csv` e carregados para uma estrutura de **lista encadeada** em memória, representada pelo TAD `BDPaciente`. O sistema permite **inserir**, **consultar**, **atualizar**, **remover** e **listar** pacientes.

Este projeto foi desenvolvido como parte do **Trabalho Prático 2** da disciplina de Programação, no curso de Sistemas de Informação do IFES - Campus Serra.

## Estrutura do Projeto

```
tp2/
├── Makefile              # Arquivo de compilação
├── README.md             # Documentação do projeto
├── bd_paciente.csv       # Arquivo de dados dos pacientes (persistência)
├── main.c                # Programa principal (interface de usuário)
├── paciente.h/.c         # TAD Paciente
├── bd_paciente.h/.c      # TAD BDPaciente (banco de dados em memória)
```

## Como Executar

### Pré-requisitos
- GCC (GNU Compiler Collection)
- Linux (testado em Ubuntu 22.04 e 24.04)
- Make (opcional, mas recomendado)

### Compilação

#### Usando Makefile:
```bash
make
```

#### Compilação manual:
```bash
gcc -Wall main.c paciente.c bd_paciente.c -o tp2
```

### Execução:
```bash
./tp2
```

Ou com Make:
```bash
make run
```

### Limpeza dos arquivos compilados:
```bash
make clean
```

## Funcionalidades

### 1. Consultar Paciente
- **Por Nome** (case-insensitive)
- **Por CPF** (considerando apenas os números)
- Permite busca por **prefixo**

### 2. Atualizar Paciente
- Busca o paciente por meio de uma consulta.
- Permite modificar CPF, nome ou idade.
- Data de cadastro se mantém.
- Campos podem ser mantidos usando o caractere `-`.

### 3. Remover Paciente
- Permite localizar um paciente via consulta.
- Exige confirmação antes de excluir o registro.
- Remove definitivamente da memória e do CSV.

### 4. Inserir Paciente
- Solicita CPF (11 dígitos), nome e idade.
- Gera `ID` automaticamente.
- Data de cadastro é preenchida com a data atual.
- Exibe confirmação antes de inserir.

### 5. Listar Todos os Pacientes
- Exibe todos os registros formatados em tabela.
- Mostra o total de pacientes.

### 6. Sair
- Salva automaticamente o estado atual no arquivo CSV.

## TADs (Tipos Abstratos de Dados)

### 1. Paciente (`paciente.h` / `paciente.c`)

**Campos**:
```c
typedef struct paciente {
    int id;
    char cpf[MAX_CPF];
    char nome[MAX_NOME];
    int idade;
    char data_cadastro[MAX_DATA];
} Paciente;
```

**Responsabilidades**:
- Representar um paciente individual.
- Getters e setters.
- Impressão formatada.
- Comparação por prefixo (nome e CPF).

### 2. BDPaciente (`bd_paciente.h` / `bd_paciente.c`)

**Representação**:
```c
typedef struct bdpaciente {
    NoPaciente *primeiro;
    int total;
} BDPaciente;
```

**Funcionalidades**:
- Carregamento e salvamento no CSV.
- Inserção, consulta, atualização e remoção.
- Contagem de pacientes.
- Busca por ID ou prefixo.
- Lista encadeada simples como estrutura base.

## Principais Decisões de Implementação

### 1. Estrutura de Dados
- **Lista encadeada** para armazenar pacientes em memória.
- Cada nó contém um ponteiro para `Paciente` e para o próximo nó.
- Implementação robusta com alocação e desalocação de memória.

### 2. Modularização
- TADs distintos para `Paciente` e `BDPaciente`.
- `main.c` contém a interface de usuário.
- Separação de responsabilidades clara entre os arquivos.

### 3. Manipulação de Arquivos
- Leitura e escrita do CSV com `fgets` e `strtok`.
- Cabeçalho do CSV é ignorado na leitura e reescrito na gravação.
- Dados são persistidos automaticamente após alterações.

### 4. Interface e Interação com Usuário
- Menus claros e informativos.
- Confirmações de ações críticas (inserção, atualização, remoção).
- Entradas robustas com validação e limpeza de buffer.
- Formatação amigável de CPF e datas.

### 5. Validação e Robustez
- CPF deve ter exatamente 11 dígitos numéricos.
- Idade deve ser positiva.
- Nome não pode ser vazio.
- Entradas inválidas são tratadas com mensagens amigáveis.

## Exemplo de Execução

```text
=== Sistema de Gerenciamento de Pacientes ===
1 - Consultar paciente
2 - Atualizar paciente
3 - Remover paciente
4 - Inserir paciente
5 - Imprimir lista de pacientes
Q - Sair
Escolha uma opção:
```
