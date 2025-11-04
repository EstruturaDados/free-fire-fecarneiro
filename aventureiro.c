/*
 * SISTEMA DE INVENTARIO - FREE FIRE (NIVEL AVENTUREIRO)
 *
 * Descricao: Sistema comparativo que implementa duas estruturas de dados
 * para gerenciar o inventario: vetor (lista sequencial) e lista encadeada.
 * O objetivo e demonstrar como a escolha da estrutura de dados influencia
 * na performance em operacoes criticas.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================================================
// DEFINICAO DE CONSTANTES
// ============================================================================

#define MAX_ITENS 100       // Capacidade maxima do vetor
#define TAM_NOME 30         // Tamanho maximo do nome do item
#define TAM_TIPO 20         // Tamanho maximo do tipo do item

// ============================================================================
// ESTRUTURAS DE DADOS
// ============================================================================

/*
 * Struct: Item
 * Representa um item do inventario com suas propriedades.
 *
 * Campos:
 *   - nome: identificador do item (ex: "AK-47", "Kit Medico")
 *   - tipo: categoria do item (arma, municao, cura, ferramenta)
 *   - quantidade: numero de unidades deste item
 */
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
} Item;

/*
 * Struct: No
 * Representa um no da lista encadeada.
 *
 * Campos:
 *   - dados: estrutura Item contendo os dados do inventario
 *   - proximo: ponteiro para o proximo no da lista
 */
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// ============================================================================
// VARIAVEIS GLOBAIS
// ============================================================================

// Estruturas para o vetor
Item inventarioVetor[MAX_ITENS];
int totalItensVetor = 0;

// Estruturas para a lista encadeada
No* inicioLista = NULL;

// Contadores de comparacoes para analise de desempenho
int comparacoesSequencial = 0;
int comparacoesBinaria = 0;

// ============================================================================
// FUNCOES AUXILIARES
// ============================================================================

/*
 * Funcao: limparBuffer
 * Limpa o buffer de entrada para evitar problemas com leitura de strings
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * Funcao: pausar
 * Pausa a execucao e aguarda o usuario pressionar ENTER
 */
void pausar() {
    printf("\nPressione ENTER para continuar...");
    limparBuffer();
    getchar();
}

// ============================================================================
// OPERACOES COM VETOR (LISTA SEQUENCIAL)
// ============================================================================

/*
 * Funcao: inserirItemVetor
 * Insere um novo item no final do vetor.
 *
 * Complexidade: O(1) - insercao no final
 *
 * Parametros:
 *   - item: estrutura Item a ser inserida
 *
 * Retorno: 1 se sucesso, 0 se falhou (vetor cheio)
 */
int inserirItemVetor(Item item) {
    if (totalItensVetor >= MAX_ITENS) {
        printf("\n[VETOR] Mochila cheia! Capacidade maxima atingida.\n");
        return 0;
    }

    inventarioVetor[totalItensVetor] = item;
    totalItensVetor++;

    printf("\n[VETOR] Item '%s' inserido com sucesso!\n", item.nome);
    return 1;
}

/*
 * Funcao: removerItemVetor
 * Remove um item do vetor pelo nome.
 *
 * Complexidade: O(n) - busca + deslocamento dos elementos
 *
 * Parametros:
 *   - nome: nome do item a ser removido
 *
 * Retorno: 1 se sucesso, 0 se nao encontrado
 */
int removerItemVetor(char* nome) {
    int encontrado = -1;

    // Busca sequencial pelo item
    for (int i = 0; i < totalItensVetor; i++) {
        if (strcmp(inventarioVetor[i].nome, nome) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("\n[VETOR] Item '%s' nao encontrado.\n", nome);
        return 0;
    }

    // Desloca todos os elementos posteriores uma posicao para tras
    for (int i = encontrado; i < totalItensVetor - 1; i++) {
        inventarioVetor[i] = inventarioVetor[i + 1];
    }

    totalItensVetor--;
    printf("\n[VETOR] Item '%s' removido com sucesso!\n", nome);
    return 1;
}

/*
 * Funcao: listarItensVetor
 * Lista todos os itens do vetor.
 *
 * Complexidade: O(n) - percorre todos os elementos
 */
void listarItensVetor() {
    printf("\n========== INVENTARIO - VETOR ==========\n");

    if (totalItensVetor == 0) {
        printf("Inventario vazio.\n");
        return;
    }

    printf("Total de itens: %d/%d\n\n", totalItensVetor, MAX_ITENS);
    printf("%-5s %-20s %-15s %-10s\n", "Pos", "Nome", "Tipo", "Qtd");
    printf("-------------------------------------------------------\n");

    for (int i = 0; i < totalItensVetor; i++) {
        printf("%-5d %-20s %-15s %-10d\n",
               i,
               inventarioVetor[i].nome,
               inventarioVetor[i].tipo,
               inventarioVetor[i].quantidade);
    }

    printf("========================================\n");
}

/*
 * Funcao: buscarSequencialVetor
 * Realiza busca sequencial no vetor.
 *
 * Complexidade: O(n) - pior caso percorre todos os elementos
 *
 * Parametros:
 *   - nome: nome do item a buscar
 *
 * Retorno: indice do item se encontrado, -1 caso contrario
 */
int buscarSequencialVetor(char* nome) {
    comparacoesSequencial = 0;

    for (int i = 0; i < totalItensVetor; i++) {
        comparacoesSequencial++;
        if (strcmp(inventarioVetor[i].nome, nome) == 0) {
            return i;
        }
    }

    return -1;
}

/*
 * Funcao: ordenarVetor
 * Ordena os itens do vetor por nome usando Selection Sort.
 *
 * Complexidade: O(n^2) - dois loops aninhados
 *
 * Algoritmo Selection Sort:
 * Para cada posicao i:
 *   1. Encontra o menor elemento entre i e o final do vetor
 *   2. Troca o elemento da posicao i com o menor encontrado
 *
 * Vantagens: simples de implementar, poucas trocas
 * Desvantagens: sempre O(n^2), mesmo com vetor ja ordenado
 */
void ordenarVetor() {
    if (totalItensVetor <= 1) {
        printf("\n[VETOR] Nada a ordenar (menos de 2 itens).\n");
        return;
    }

    clock_t inicio = clock();

    for (int i = 0; i < totalItensVetor - 1; i++) {
        int menorIndice = i;

        // Encontra o menor elemento no restante do vetor
        for (int j = i + 1; j < totalItensVetor; j++) {
            if (strcmp(inventarioVetor[j].nome, inventarioVetor[menorIndice].nome) < 0) {
                menorIndice = j;
            }
        }

        // Troca o elemento atual com o menor encontrado
        if (menorIndice != i) {
            Item temp = inventarioVetor[i];
            inventarioVetor[i] = inventarioVetor[menorIndice];
            inventarioVetor[menorIndice] = temp;
        }
    }

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;

    printf("\n[VETOR] Itens ordenados com sucesso!\n");
    printf("[VETOR] Tempo de ordenacao: %.4f ms\n", tempo);
}

/*
 * Funcao: buscarBinariaVetor
 * Realiza busca binaria no vetor (REQUER VETOR ORDENADO).
 *
 * Complexidade: O(log n) - divide o espaco de busca pela metade a cada passo
 *
 * Algoritmo:
 * Enquanto houver elementos a pesquisar:
 *   1. Calcula o elemento do meio
 *   2. Se o meio e o elemento procurado, retorna
 *   3. Se o elemento e menor que o meio, busca na metade esquerda
 *   4. Se o elemento e maior que o meio, busca na metade direita
 *
 * IMPORTANTE: Vetor DEVE estar ordenado para funcionar corretamente!
 *
 * Parametros:
 *   - nome: nome do item a buscar
 *
 * Retorno: indice do item se encontrado, -1 caso contrario
 */
int buscarBinariaVetor(char* nome) {
    comparacoesBinaria = 0;

    int esquerda = 0;
    int direita = totalItensVetor - 1;

    while (esquerda <= direita) {
        int meio = (esquerda + direita) / 2;
        comparacoesBinaria++;

        int resultado = strcmp(nome, inventarioVetor[meio].nome);

        if (resultado == 0) {
            return meio;  // Encontrou!
        } else if (resultado < 0) {
            direita = meio - 1;  // Busca na metade esquerda
        } else {
            esquerda = meio + 1;  // Busca na metade direita
        }
    }

    return -1;  // Nao encontrou
}

// ============================================================================
// OPERACOES COM LISTA ENCADEADA
// ============================================================================

/*
 * Funcao: inserirItemLista
 * Insere um novo item no final da lista encadeada.
 *
 * Complexidade: O(n) - precisa percorrer ate o final para inserir
 * (poderia ser O(1) se mantivessemos um ponteiro para o ultimo no)
 *
 * Parametros:
 *   - item: estrutura Item a ser inserida
 */
void inserirItemLista(Item item) {
    // Aloca memoria para o novo no
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("\n[LISTA] Erro ao alocar memoria!\n");
        return;
    }

    novoNo->dados = item;
    novoNo->proximo = NULL;

    // Se a lista esta vazia, o novo no e o primeiro
    if (inicioLista == NULL) {
        inicioLista = novoNo;
    } else {
        // Percorre ate o final da lista
        No* atual = inicioLista;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novoNo;
    }

    printf("\n[LISTA] Item '%s' inserido com sucesso!\n", item.nome);
}

/*
 * Funcao: removerItemLista
 * Remove um item da lista encadeada pelo nome.
 *
 * Complexidade: O(n) - busca pelo elemento
 *
 * Parametros:
 *   - nome: nome do item a ser removido
 *
 * Retorno: 1 se sucesso, 0 se nao encontrado
 */
int removerItemLista(char* nome) {
    if (inicioLista == NULL) {
        printf("\n[LISTA] Lista vazia!\n");
        return 0;
    }

    No* atual = inicioLista;
    No* anterior = NULL;

    // Busca o no a ser removido
    while (atual != NULL && strcmp(atual->dados.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    // Se nao encontrou
    if (atual == NULL) {
        printf("\n[LISTA] Item '%s' nao encontrado.\n", nome);
        return 0;
    }

    // Se e o primeiro no
    if (anterior == NULL) {
        inicioLista = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual);
    printf("\n[LISTA] Item '%s' removido com sucesso!\n", nome);
    return 1;
}

/*
 * Funcao: listarItensLista
 * Lista todos os itens da lista encadeada.
 *
 * Complexidade: O(n) - percorre todos os nos
 */
void listarItensLista() {
    printf("\n========== INVENTARIO - LISTA ENCADEADA ==========\n");

    if (inicioLista == NULL) {
        printf("Inventario vazio.\n");
        return;
    }

    printf("%-5s %-20s %-15s %-10s\n", "Pos", "Nome", "Tipo", "Qtd");
    printf("-------------------------------------------------------\n");

    No* atual = inicioLista;
    int posicao = 0;

    while (atual != NULL) {
        printf("%-5d %-20s %-15s %-10d\n",
               posicao,
               atual->dados.nome,
               atual->dados.tipo,
               atual->dados.quantidade);
        atual = atual->proximo;
        posicao++;
    }

    printf("==================================================\n");
}

/*
 * Funcao: buscarItemLista
 * Realiza busca sequencial na lista encadeada.
 *
 * Complexidade: O(n) - precisa percorrer os nos sequencialmente
 * Nota: Lista encadeada NAO permite busca binaria!
 *
 * Parametros:
 *   - nome: nome do item a buscar
 *
 * Retorno: ponteiro para o no se encontrado, NULL caso contrario
 */
No* buscarItemLista(char* nome) {
    comparacoesSequencial = 0;

    No* atual = inicioLista;

    while (atual != NULL) {
        comparacoesSequencial++;
        if (strcmp(atual->dados.nome, nome) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }

    return NULL;
}

/*
 * Funcao: liberarLista
 * Libera toda a memoria alocada pela lista encadeada.
 *
 * Importante: sempre liberar a memoria ao final do programa
 * para evitar memory leaks!
 */
void liberarLista() {
    No* atual = inicioLista;
    No* proximo;

    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    inicioLista = NULL;
}

// ============================================================================
// FUNCOES DE INTERFACE E MENUS
// ============================================================================

/*
 * Funcao: lerItem
 * Le os dados de um item do usuario.
 *
 * Retorno: estrutura Item preenchida
 */
Item lerItem() {
    Item item;

    printf("\nDigite o nome do item: ");
    limparBuffer();
    fgets(item.nome, TAM_NOME, stdin);
    item.nome[strcspn(item.nome, "\n")] = '\0';

    printf("Digite o tipo (arma/municao/cura/ferramenta): ");
    fgets(item.tipo, TAM_TIPO, stdin);
    item.tipo[strcspn(item.tipo, "\n")] = '\0';

    printf("Digite a quantidade: ");
    scanf("%d", &item.quantidade);

    return item;
}

/*
 * Funcao: menuVetor
 * Menu de operacoes com vetor
 */
void menuVetor() {
    int opcao;

    do {
        printf("\n");
        printf("========================================\n");
        printf("   MENU - VETOR (LISTA SEQUENCIAL)\n");
        printf("========================================\n");
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item (sequencial)\n");
        printf("5. Ordenar itens por nome\n");
        printf("6. Buscar item (binaria - requer ordenacao)\n");
        printf("0. Voltar\n");
        printf("========================================\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Item item = lerItem();
                inserirItemVetor(item);
                break;
            }
            case 2: {
                char nome[TAM_NOME];
                printf("\nDigite o nome do item a remover: ");
                limparBuffer();
                fgets(nome, TAM_NOME, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                removerItemVetor(nome);
                break;
            }
            case 3:
                listarItensVetor();
                break;
            case 4: {
                char nome[TAM_NOME];
                printf("\nDigite o nome do item a buscar: ");
                limparBuffer();
                fgets(nome, TAM_NOME, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                clock_t inicio = clock();
                int indice = buscarSequencialVetor(nome);
                clock_t fim = clock();
                double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;

                if (indice != -1) {
                    printf("\n[VETOR] Item encontrado na posicao %d\n", indice);
                    printf("Nome: %s | Tipo: %s | Qtd: %d\n",
                           inventarioVetor[indice].nome,
                           inventarioVetor[indice].tipo,
                           inventarioVetor[indice].quantidade);
                } else {
                    printf("\n[VETOR] Item nao encontrado.\n");
                }
                printf("[VETOR] Comparacoes: %d\n", comparacoesSequencial);
                printf("[VETOR] Tempo: %.4f ms\n", tempo);
                break;
            }
            case 5:
                ordenarVetor();
                break;
            case 6: {
                char nome[TAM_NOME];
                printf("\nDigite o nome do item a buscar: ");
                limparBuffer();
                fgets(nome, TAM_NOME, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                clock_t inicio = clock();
                int indice = buscarBinariaVetor(nome);
                clock_t fim = clock();
                double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;

                if (indice != -1) {
                    printf("\n[VETOR] Item encontrado na posicao %d\n", indice);
                    printf("Nome: %s | Tipo: %s | Qtd: %d\n",
                           inventarioVetor[indice].nome,
                           inventarioVetor[indice].tipo,
                           inventarioVetor[indice].quantidade);
                } else {
                    printf("\n[VETOR] Item nao encontrado.\n");
                }
                printf("[VETOR] Comparacoes: %d\n", comparacoesBinaria);
                printf("[VETOR] Tempo: %.4f ms\n", tempo);
                printf("\nNOTA: Se o resultado for inesperado, verifique se o vetor esta ordenado!\n");
                break;
            }
            case 0:
                break;
            default:
                printf("\nOpcao invalida!\n");
        }

        if (opcao != 0) {
            pausar();
        }

    } while (opcao != 0);
}

/*
 * Funcao: menuLista
 * Menu de operacoes com lista encadeada
 */
void menuLista() {
    int opcao;

    do {
        printf("\n");
        printf("========================================\n");
        printf("   MENU - LISTA ENCADEADA\n");
        printf("========================================\n");
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item (sequencial)\n");
        printf("0. Voltar\n");
        printf("========================================\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Item item = lerItem();
                inserirItemLista(item);
                break;
            }
            case 2: {
                char nome[TAM_NOME];
                printf("\nDigite o nome do item a remover: ");
                limparBuffer();
                fgets(nome, TAM_NOME, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                removerItemLista(nome);
                break;
            }
            case 3:
                listarItensLista();
                break;
            case 4: {
                char nome[TAM_NOME];
                printf("\nDigite o nome do item a buscar: ");
                limparBuffer();
                fgets(nome, TAM_NOME, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                clock_t inicio = clock();
                No* no = buscarItemLista(nome);
                clock_t fim = clock();
                double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;

                if (no != NULL) {
                    printf("\n[LISTA] Item encontrado!\n");
                    printf("Nome: %s | Tipo: %s | Qtd: %d\n",
                           no->dados.nome,
                           no->dados.tipo,
                           no->dados.quantidade);
                } else {
                    printf("\n[LISTA] Item nao encontrado.\n");
                }
                printf("[LISTA] Comparacoes: %d\n", comparacoesSequencial);
                printf("[LISTA] Tempo: %.4f ms\n", tempo);
                break;
            }
            case 0:
                break;
            default:
                printf("\nOpcao invalida!\n");
        }

        if (opcao != 0) {
            pausar();
        }

    } while (opcao != 0);
}

/*
 * Funcao: compararDesempenho
 * Compara o desempenho de busca entre vetor e lista encadeada
 */
void compararDesempenho() {
    printf("\n");
    printf("========================================\n");
    printf("   COMPARACAO DE DESEMPENHO\n");
    printf("========================================\n");

    char nome[TAM_NOME];
    printf("\nDigite o nome do item para comparar buscas: ");
    limparBuffer();
    fgets(nome, TAM_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("\n--- BUSCA SEQUENCIAL ---\n");

    // Busca no vetor
    clock_t inicio = clock();
    int indiceVetor = buscarSequencialVetor(nome);
    clock_t fim = clock();
    double tempoVetor = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;
    int comparacoesVetor = comparacoesSequencial;

    printf("\nVETOR:\n");
    printf("  Resultado: %s\n", indiceVetor != -1 ? "ENCONTRADO" : "NAO ENCONTRADO");
    printf("  Comparacoes: %d\n", comparacoesVetor);
    printf("  Tempo: %.4f ms\n", tempoVetor);

    // Busca na lista
    inicio = clock();
    No* noLista = buscarItemLista(nome);
    fim = clock();
    double tempoLista = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;
    int comparacoesLista = comparacoesSequencial;

    printf("\nLISTA ENCADEADA:\n");
    printf("  Resultado: %s\n", noLista != NULL ? "ENCONTRADO" : "NAO ENCONTRADO");
    printf("  Comparacoes: %d\n", comparacoesLista);
    printf("  Tempo: %.4f ms\n", tempoLista);

    printf("\n--- BUSCA BINARIA (VETOR ORDENADO) ---\n");
    printf("\nNOTA: Certifique-se de que o vetor esta ordenado!\n");

    inicio = clock();
    int indiceBinaria = buscarBinariaVetor(nome);
    fim = clock();
    double tempoBinaria = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;

    printf("\nVETOR (Busca Binaria):\n");
    printf("  Resultado: %s\n", indiceBinaria != -1 ? "ENCONTRADO" : "NAO ENCONTRADO");
    printf("  Comparacoes: %d\n", comparacoesBinaria);
    printf("  Tempo: %.4f ms\n", tempoBinaria);

    printf("\n========================================\n");
    printf("   ANALISE COMPARATIVA\n");
    printf("========================================\n");
    printf("\nBusca Sequencial:\n");
    printf("  Vetor vs Lista: %s\n",
           comparacoesVetor < comparacoesLista ? "Vetor mais eficiente" :
           comparacoesVetor > comparacoesLista ? "Lista mais eficiente" :
           "Desempenho similar");

    printf("\nBusca Binaria vs Sequencial (vetor):\n");
    printf("  Ganho de comparacoes: %d -> %d (%.1f%% de reducao)\n",
           comparacoesVetor, comparacoesBinaria,
           comparacoesVetor > 0 ? (1.0 - (double)comparacoesBinaria/comparacoesVetor) * 100 : 0);

    printf("\nCONCLUSAO:\n");
    printf("  - Busca sequencial: similar em vetor e lista\n");
    printf("  - Busca binaria: MUITO mais eficiente (requer vetor ordenado)\n");
    printf("  - Lista encadeada NAO permite busca binaria!\n");

    pausar();
}

// ============================================================================
// FUNCAO PRINCIPAL
// ============================================================================

int main() {
    int opcao;

    printf("\n");
    printf("========================================\n");
    printf("  SISTEMA DE INVENTARIO - NIVEL AVENTUREIRO\n");
    printf("  Comparacao: Vetor vs Lista Encadeada\n");
    printf("========================================\n");

    do {
        printf("\n");
        printf("========================================\n");
        printf("           MENU PRINCIPAL\n");
        printf("========================================\n");
        printf("1. Operacoes com VETOR\n");
        printf("2. Operacoes com LISTA ENCADEADA\n");
        printf("3. Comparar desempenho\n");
        printf("0. Sair\n");
        printf("========================================\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                menuVetor();
                break;
            case 2:
                menuLista();
                break;
            case 3:
                compararDesempenho();
                break;
            case 0:
                printf("\n Encerrando sistema...\n");
                printf(" Boa sorte no campo de batalha!\n\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
                pausar();
        }

    } while (opcao != 0);

    // Libera memoria da lista encadeada
    liberarLista();

    return 0;
}
