# Compilador e flags
CC = gcc
CFLAGS = -Wall 

# Nome do executável
TARGET = tp1

# Arquivos fonte
SOURCES = main.c paciente.c bd_paciente.c

# Arquivos objeto
OBJECTS = $(SOURCES:.c=.o)

# Arquivos de cabeçalho
HEADERS = paciente.h bd_paciente.h

# Regra principal
all: $(TARGET)

# Compilação do executável
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compilação dos arquivos objeto
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza dos arquivos gerados
clean:
	rm -f $(OBJECTS) $(TARGET)

# Execução do programa
run: $(TARGET)
	./$(TARGET)

# Regras que não correspondem a arquivos
.PHONY: all clean run valgrind