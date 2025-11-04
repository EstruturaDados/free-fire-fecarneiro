#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Código da Ilha – Edição Free Fire
// Nível: Mestre - Desafio Final
// Sistema de priorização e montagem de componentes da torre de fuga
// Implementa diferentes algoritmos de ordenação, busca binária e análise de desempenho

// Struct Componente: representa um item necessário para a montagem da torre
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;  // de 1 a 10
} Componente;

// Variáveis globais para medição de desempenho
int comparacoes = 0;

// Função para limpar buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para exibir todos os componentes formatados
void mostrarComponentes(Componente componentes[], int n) {
    if (n == 0) {
        printf("\nNenhum componente cadastrado ainda.\n");
        return;
    }

    printf("\n╔════════════════════════════════════════════════════════════════════════╗\n");
    printf("║              COMPONENTES DA TORRE DE FUGA                              ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ %-28s ║ %-18s ║ %-10s ║\n", "NOME", "TIPO", "PRIORIDADE");
    printf("╠════════════════════════════════════════════════════════════════════════╣\n");

    for (int i = 0; i < n; i++) {
        printf("║ %-28s ║ %-18s ║ %-10d ║\n",
               componentes[i].nome,
               componentes[i].tipo,
               componentes[i].prioridade);
    }

    printf("╚════════════════════════════════════════════════════════════════════════╝\n");
}

// Bubble Sort - Ordenação por nome (string)
void bubbleSortNome(Componente componentes[], int n) {
    comparacoes = 0;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparacoes++;
            if (strcmp(componentes[j].nome, componentes[j + 1].nome) > 0) {
                // Troca os componentes
                Componente temp = componentes[j];
                componentes[j] = componentes[j + 1];
                componentes[j + 1] = temp;
            }
        }
    }
}

// Insertion Sort - Ordenação por tipo (string)
void insertionSortTipo(Componente componentes[], int n) {
    comparacoes = 0;

    for (int i = 1; i < n; i++) {
        Componente chave = componentes[i];
        int j = i - 1;

        while (j >= 0) {
            comparacoes++;
            if (strcmp(componentes[j].tipo, chave.tipo) > 0) {
                componentes[j + 1] = componentes[j];
                j--;
            } else {
                break;
            }
        }
        componentes[j + 1] = chave;
    }
}

// Selection Sort - Ordenação por prioridade (int, ordem decrescente)
void selectionSortPrioridade(Componente componentes[], int n) {
    comparacoes = 0;

    for (int i = 0; i < n - 1; i++) {
        int maxIdx = i;

        for (int j = i + 1; j < n; j++) {
            comparacoes++;
            // Ordem decrescente (maior prioridade primeiro)
            if (componentes[j].prioridade > componentes[maxIdx].prioridade) {
                maxIdx = j;
            }
        }

        if (maxIdx != i) {
            // Troca os componentes
            Componente temp = componentes[i];
            componentes[i] = componentes[maxIdx];
            componentes[maxIdx] = temp;
        }
    }
}

// Busca Binária - Localiza componente por nome (requer ordenação prévia por nome)
int buscaBinariaPorNome(Componente componentes[], int n, char nomeBuscado[]) {
    int esquerda = 0;
    int direita = n - 1;
    int numComparacoes = 0;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        numComparacoes++;

        int resultado = strcmp(componentes[meio].nome, nomeBuscado);

        if (resultado == 0) {
            printf("\n✓ Componente-chave encontrado em %d comparações!\n", numComparacoes);
            printf("\nDetalhes:\n");
            printf("  Nome: %s\n", componentes[meio].nome);
            printf("  Tipo: %s\n", componentes[meio].tipo);
            printf("  Prioridade: %d\n", componentes[meio].prioridade);
            return meio;
        }

        if (resultado < 0) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }

    printf("\n✗ Componente não encontrado após %d comparações.\n", numComparacoes);
    return -1;
}

// Função para cadastrar componentes
void cadastrarComponentes(Componente componentes[], int *n) {
    if (*n >= 20) {
        printf("\n✗ Limite de componentes atingido (20)!\n");
        return;
    }

    printf("\n═══════════════════════════════════════\n");
    printf("    CADASTRO DE COMPONENTE\n");
    printf("═══════════════════════════════════════\n");

    printf("\nNome do componente: ");
    fgets(componentes[*n].nome, 30, stdin);
    componentes[*n].nome[strcspn(componentes[*n].nome, "\n")] = 0;  // Remove \n

    printf("Tipo (ex: controle, suporte, propulsão): ");
    fgets(componentes[*n].tipo, 20, stdin);
    componentes[*n].tipo[strcspn(componentes[*n].tipo, "\n")] = 0;

    printf("Prioridade (1-10): ");
    scanf("%d", &componentes[*n].prioridade);
    limparBuffer();

    // Validação de prioridade
    if (componentes[*n].prioridade < 1 || componentes[*n].prioridade > 10) {
        printf("✗ Prioridade inválida. Usando 5 como padrão.\n");
        componentes[*n].prioridade = 5;
    }

    (*n)++;
    printf("\n✓ Componente cadastrado com sucesso!\n");
}

// Menu de ordenação
void menuOrdenacao(Componente componentes[], int n, int *ordenadoPorNome) {
    if (n == 0) {
        printf("\n✗ Nenhum componente cadastrado para ordenar.\n");
        return;
    }

    int opcao;
    clock_t inicio, fim;
    double tempoExecucao;

    printf("\n═══════════════════════════════════════\n");
    printf("    ESTRATÉGIAS DE ORDENAÇÃO\n");
    printf("═══════════════════════════════════════\n");
    printf("1. Bubble Sort (por nome)\n");
    printf("2. Insertion Sort (por tipo)\n");
    printf("3. Selection Sort (por prioridade)\n");
    printf("0. Voltar\n");
    printf("═══════════════════════════════════════\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    limparBuffer();

    switch (opcao) {
        case 1:
            printf("\nExecutando Bubble Sort...\n");
            inicio = clock();
            bubbleSortNome(componentes, n);
            fim = clock();
            tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;

            printf("✓ Ordenação concluída!\n");
            printf("  Algoritmo: Bubble Sort\n");
            printf("  Critério: Nome (ordem alfabética)\n");
            printf("  Comparações: %d\n", comparacoes);
            printf("  Tempo: %.4f ms\n", tempoExecucao);

            *ordenadoPorNome = 1;
            mostrarComponentes(componentes, n);
            break;

        case 2:
            printf("\nExecutando Insertion Sort...\n");
            inicio = clock();
            insertionSortTipo(componentes, n);
            fim = clock();
            tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;

            printf("✓ Ordenação concluída!\n");
            printf("  Algoritmo: Insertion Sort\n");
            printf("  Critério: Tipo (ordem alfabética)\n");
            printf("  Comparações: %d\n", comparacoes);
            printf("  Tempo: %.4f ms\n", tempoExecucao);

            *ordenadoPorNome = 0;
            mostrarComponentes(componentes, n);
            break;

        case 3:
            printf("\nExecutando Selection Sort...\n");
            inicio = clock();
            selectionSortPrioridade(componentes, n);
            fim = clock();
            tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;

            printf("✓ Ordenação concluída!\n");
            printf("  Algoritmo: Selection Sort\n");
            printf("  Critério: Prioridade (maior para menor)\n");
            printf("  Comparações: %d\n", comparacoes);
            printf("  Tempo: %.4f ms\n", tempoExecucao);

            *ordenadoPorNome = 0;
            mostrarComponentes(componentes, n);
            break;

        case 0:
            return;

        default:
            printf("\n✗ Opção inválida!\n");
    }
}

// Função de busca com menu
void realizarBusca(Componente componentes[], int n, int ordenadoPorNome) {
    if (n == 0) {
        printf("\n✗ Nenhum componente cadastrado para buscar.\n");
        return;
    }

    if (!ordenadoPorNome) {
        printf("\n✗ ATENÇÃO: A busca binária requer ordenação por nome!\n");
        printf("Por favor, ordene os componentes usando Bubble Sort primeiro.\n");
        return;
    }

    char nomeBuscado[30];
    printf("\n═══════════════════════════════════════\n");
    printf("    BUSCA DO COMPONENTE-CHAVE\n");
    printf("═══════════════════════════════════════\n");
    printf("\nNome do componente a buscar: ");
    fgets(nomeBuscado, 30, stdin);
    nomeBuscado[strcspn(nomeBuscado, "\n")] = 0;

    buscaBinariaPorNome(componentes, n, nomeBuscado);
}

// Função principal
int main() {
    Componente componentes[20];
    int numComponentes = 0;
    int ordenadoPorNome = 0;  // Flag para controlar se está ordenado por nome
    int opcao;

    printf("╔════════════════════════════════════════════════════════════════════════╗\n");
    printf("║           CÓDIGO DA ILHA - EDIÇÃO FREE FIRE                            ║\n");
    printf("║           NÍVEL MESTRE: TORRE DE FUGA                                  ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════╝\n");

    printf("\nPrepare-se para o desafio final! A última safe zone está se fechando.\n");
    printf("Monte a torre de resgate com os componentes certos, na ordem exata!\n");

    do {
        printf("\n\n");
        printf("═══════════════════════════════════════\n");
        printf("         MENU PRINCIPAL\n");
        printf("═══════════════════════════════════════\n");
        printf("1. Cadastrar componente\n");
        printf("2. Listar componentes\n");
        printf("3. Ordenar componentes\n");
        printf("4. Buscar componente-chave (busca binária)\n");
        printf("5. Status da montagem\n");
        printf("0. Sair\n");
        printf("═══════════════════════════════════════\n");
        printf("Componentes cadastrados: %d/20\n", numComponentes);
        printf("Ordenado por nome: %s\n", ordenadoPorNome ? "Sim" : "Não");
        printf("═══════════════════════════════════════\n");
        printf("Escolha: ");

        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrarComponentes(componentes, &numComponentes);
                ordenadoPorNome = 0;  // Ao adicionar, perde ordenação
                break;

            case 2:
                mostrarComponentes(componentes, numComponentes);
                break;

            case 3:
                menuOrdenacao(componentes, numComponentes, &ordenadoPorNome);
                break;

            case 4:
                realizarBusca(componentes, numComponentes, ordenadoPorNome);
                break;

            case 5:
                printf("\n═══════════════════════════════════════\n");
                printf("    STATUS DA MONTAGEM DA TORRE\n");
                printf("═══════════════════════════════════════\n");
                printf("Total de componentes: %d/20\n", numComponentes);
                printf("Sistema ordenado: %s\n", ordenadoPorNome ? "Sim (pronto para busca)" : "Não");

                if (numComponentes >= 10 && ordenadoPorNome) {
                    printf("\n✓ SISTEMA PRONTO PARA ATIVAÇÃO!\n");
                    printf("Você pode buscar o componente-chave para iniciar a fuga.\n");
                } else if (numComponentes < 10) {
                    printf("\n⚠ Componentes insuficientes. Mínimo recomendado: 10\n");
                } else {
                    printf("\n⚠ Ordene os componentes antes de ativar a torre.\n");
                }
                break;

            case 0:
                printf("\n╔════════════════════════════════════════════════════════════════════════╗\n");
                printf("║  Missão encerrada. Boa sorte na fuga da ilha!                         ║\n");
                printf("╚════════════════════════════════════════════════════════════════════════╝\n");
                break;

            default:
                printf("\n✗ Opção inválida! Tente novamente.\n");
        }

        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 0);

    return 0;
}
