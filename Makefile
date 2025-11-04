# Makefile para o Sistema de Inventário Free Fire

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Alvos
TARGET_NOVATO = inventario
TARGET_AVENTUREIRO = aventureiro
TARGET_MESTRE = mestre
SRC_NOVATO = inventario.c
SRC_AVENTUREIRO = aventureiro.c
SRC_MESTRE = mestre.c

# Regra padrão: compila todos os programas
all: $(TARGET_NOVATO) $(TARGET_AVENTUREIRO) $(TARGET_MESTRE)

# Compila o programa nível novato
$(TARGET_NOVATO): $(SRC_NOVATO)
	@echo "Compilando $(TARGET_NOVATO)..."
	$(CC) $(CFLAGS) -o $(TARGET_NOVATO) $(SRC_NOVATO)
	@echo "Compilacao concluida: ./$(TARGET_NOVATO)"

# Compila o programa nível aventureiro
$(TARGET_AVENTUREIRO): $(SRC_AVENTUREIRO)
	@echo "Compilando $(TARGET_AVENTUREIRO)..."
	$(CC) $(CFLAGS) -o $(TARGET_AVENTUREIRO) $(SRC_AVENTUREIRO)
	@echo "Compilacao concluida: ./$(TARGET_AVENTUREIRO)"

# Compila o programa nível mestre
$(TARGET_MESTRE): $(SRC_MESTRE)
	@echo "Compilando $(TARGET_MESTRE)..."
	$(CC) $(CFLAGS) -o $(TARGET_MESTRE) $(SRC_MESTRE)
	@echo "Compilacao concluida: ./$(TARGET_MESTRE)"

# Compila e executa o nível novato
run-novato: $(TARGET_NOVATO)
	@echo "Executando nivel novato..."
	@./$(TARGET_NOVATO)

# Compila e executa o nível aventureiro
run-aventureiro: $(TARGET_AVENTUREIRO)
	@echo "Executando nivel aventureiro..."
	@./$(TARGET_AVENTUREIRO)

# Compila e executa o nível mestre
run-mestre: $(TARGET_MESTRE)
	@echo "Executando nivel mestre..."
	@./$(TARGET_MESTRE)

# Remove arquivos compilados
clean:
	@echo "Removendo arquivos compilados..."
	@rm -f $(TARGET_NOVATO) $(TARGET_AVENTUREIRO) $(TARGET_MESTRE)
	@echo "Limpeza concluida!"

# Ajuda
help:
	@echo "Comandos disponiveis:"
	@echo "  make                - Compila todos os programas"
	@echo "  make inventario     - Compila apenas o nivel novato"
	@echo "  make aventureiro    - Compila apenas o nivel aventureiro"
	@echo "  make mestre         - Compila apenas o nivel mestre"
	@echo "  make run-novato     - Compila e executa o nivel novato"
	@echo "  make run-aventureiro - Compila e executa o nivel aventureiro"
	@echo "  make run-mestre     - Compila e executa o nivel mestre"
	@echo "  make clean          - Remove arquivos compilados"
	@echo "  make help           - Mostra esta ajuda"

.PHONY: all run-novato run-aventureiro run-mestre clean help
