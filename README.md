# HealthSys - Sistema de Gerenciamento de Pacientes

## Descrição

O repositório contém um sistema simplificado de gerenciamento de dados de pacientes desenvolvido em linguagem C. O sistema permite consultar e listar registros de pacientes armazenados em um arquivo CSV.

## Estrutura do Projeto

```
tp1/
├── Makefile              # Arquivo de compilação
├── README.md            # Documentação do projeto
├── bd_paciente.csv      # Arquivo de dados dos pacientes
├── main.c               # Programa principal
├── paciente.h           # Definições da estrutura Paciente
├── paciente.c           # Implementação das funções de Paciente
├── bd_paciente.h        # Definições do TAD BDPaciente
└── bd_paciente.c        # Implementação do TAD BDPaciente
```

## Como Executar

### Pré-requisitos
- GCC (GNU Compiler Collection)
- Sistema Linux (testado no Ubuntu 24.04)
- Make (opcional, mas recomendado)

### Compilação

#### Usando Makefile (recomendado):
```bash
make
```

#### Compilação manual:
```bash
gcc -Wall main.c paciente.c bd_paciente.c -o tp1
```

### Execução
```bash
./tp1
```

Ou usando o Makefile:
```bash
make run
```

### Limpeza dos Arquivos Compilados
```bash
make clean
```

## Funcionalidades

### 1. Consultar Paciente
- **Busca por Nome**: Permite buscar pacientes cujo nome comece com um prefixo específico
- **Busca por CPF**: Permite buscar pacientes cujo CPF comece com um prefixo específico
- A busca por nome é case-insensitive
- Retorna todos os registros que correspondam ao prefixo informado

### 2. Listar Todos os Pacientes
- Exibe todos os pacientes cadastrados no sistema
- Apresenta os dados organizados em formato tabular
- Mostra o total de pacientes encontrados

## TADs (Tipos Abstratos de Dados)

### 1. Paciente
**Arquivo**: `paciente.h` / `paciente.c`

**Estrutura**:
```c
typedef struct {
    int id;
    char cpf[MAX_CPF];
    char nome[MAX_NOME];
    int idade;
    char data_cadastro[MAX_DATA];
} Paciente;
```

**Responsabilidades**:
- Representar um paciente individual
- Formatação e impressão de dados do paciente
- Operações de comparação (nome e CPF com prefixo)

### 2. BDPaciente
**Arquivo**: `bd_paciente.h` / `bd_paciente.c`

**Estrutura**:
```c
typedef struct {
    Paciente *pacientes;  // Vetor de pacientes
    int total;           // Número atual de pacientes
    int capacidade;      // Capacidade máxima
} BDPaciente;
```

**Responsabilidades**:
- Gerenciar a coleção de pacientes
- Carregar dados do arquivo CSV
- Operações de consulta e listagem
- Gerenciamento de memória dinâmica

## Principais Decisões de Implementação

### 1. Estrutura de Dados
- **Vetor Dinâmico**: Utilizado array alocado dinamicamente para armazenar os pacientes
- **Capacidade Fixa**: Definida como 20 pacientes (MAX_PACIENTES)
- **Modularização**: Separação clara entre a estrutura Paciente e o gerenciador BDPaciente

### 2. Manipulação de Arquivos
- **Parsing Manual**: Implementação própria para leitura do CSV usando `strtok()`
- **Validação**: Verificação da integridade dos dados durante o carregamento
- **Tratamento de Erros**: Mensagens informativas em caso de problemas

### 3. Busca por Prefixo
- **Nome**: Busca case-insensitive usando conversão para minúsculas
- **CPF**: Busca considerando apenas numeros, sendo desnecessário a inclusão de pontos e hífens na busca por CPF.
- **Algoritmo**: Uso de `strncmp()` para comparação eficiente

### 4. Interface do Usuário
- **Menu Hierárquico**: Menu principal com submenu de consultas
- **Entrada Robusta**: Limpeza de buffer e tratamento de entrada inválida
- **Feedback Claro**: Mensagens informativas sobre resultados das operações

### 5. Gerenciamento de Memória
- **Alocação Dinâmica**: Uso de `malloc()` para flexibilidade
- **Liberação Adequada**: Função `bd_paciente_free()` para evitar vazamentos
- **Verificação de Erros**: Validação de erros de alocação, evitando que o programa rode com estruturas mal alocadas.