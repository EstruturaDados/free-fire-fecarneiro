#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Codigo da Ilha - Edicao Free Fire
// Nivel: Mestre
// Este programa simula o gerenciamento avancado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenacao com criterios e busca binaria para otimizar a gestao dos recursos.

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
    int prioridade;  // de 1 a 5
} Item;

// Enum CriterioOrdenacao:
// Define os criterios possiveis para a ordenacao dos itens (nome, tipo ou prioridade).
typedef enum {
    NOME,
    TIPO,
    PRIORIDADE
} CriterioOrdenacao;

// Vetor mochila:
// Armazena até 10 itens coletados.
Item mochila[10];

// Variaveis de controle:
// numItens: quantidade atual de itens na mochila
// comparacoes: contador para analise de desempenho
// ordenadaPorNome: controle para permitir busca binaria apenas quando ordenada por nome
int numItens = 0;
int comparacoes = 0;
bool ordenadaPorNome = false;

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.
void limparTela() {
    for (int i = 0; i < 50; i++) {
        printf("\n");
    }
}

// Funcao auxiliar para limpar buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenacao.
void exibirMenu() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║        CODIGO DA ILHA - EDICAO FREE FIRE (MESTRE)         ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("  1. Adicionar um item\n");
    printf("  2. Remover um item\n");
    printf("  3. Listar todos os itens\n");
    printf("  4. Ordenar os itens por criterio (nome, tipo, prioridade)\n");
    printf("  5. Realizar busca binaria por nome\n");
    printf("  0. Sair\n");
    printf("────────────────────────────────────────────────────────────\n");
    printf("  Itens na mochila: %d/10\n", numItens);
    printf("  Ordenada por nome: %s\n", ordenadaPorNome ? "SIM" : "NAO");
    printf("────────────────────────────────────────────────────────────\n");
    printf("  Escolha uma opcao: ");
}

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".
void inserirItem() {
    if (numItens >= 10) {
        printf("\n[ERRO] Mochila cheia! Capacidade maxima atingida (10 itens).\n");
        return;
    }

    printf("\n┌─────────────────────────────────────┐\n");
    printf("│   ADICIONAR NOVO ITEM À MOCHILA     │\n");
    printf("└─────────────────────────────────────┘\n");

    Item novoItem;

    // Leitura do nome
    printf("Nome do item: ");
    limparBuffer();
    fgets(novoItem.nome, 30, stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = '\0';

    // Leitura do tipo
    printf("Tipo do item: ");
    fgets(novoItem.tipo, 20, stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = '\0';

    // Leitura da quantidade
    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);

    // Leitura da prioridade (1 a 5)
    printf("Prioridade (1-5): ");
    scanf("%d", &novoItem.prioridade);

    // Validacao da prioridade
    if (novoItem.prioridade < 1 || novoItem.prioridade > 5) {
        printf("[AVISO] Prioridade invalida! Usando 3 como padrao.\n");
        novoItem.prioridade = 3;
    }

    // Armazena o item no vetor
    mochila[numItens] = novoItem;
    numItens++;

    // Ao inserir um novo item, a mochila deixa de estar ordenada por nome
    ordenadaPorNome = false;

    printf("\n[OK] Item '%s' adicionado com sucesso!\n", novoItem.nome);
}

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.
void removerItem() {
    if (numItens == 0) {
        printf("\n[AVISO] Mochila vazia! Nao ha itens para remover.\n");
        return;
    }

    char nomeBusca[30];
    printf("\n┌─────────────────────────────────────┐\n");
    printf("│     REMOVER ITEM DA MOCHILA         │\n");
    printf("└─────────────────────────────────────┘\n");
    printf("Nome do item a remover: ");
    limparBuffer();
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    // Busca sequencial pelo item
    int encontrado = -1;
    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontrado = i;
            break;
        }
    }

    // Se nao encontrou
    if (encontrado == -1) {
        printf("\n[ERRO] Item '%s' nao encontrado na mochila.\n", nomeBusca);
        return;
    }

    // Remove reorganizando o vetor
    for (int i = encontrado; i < numItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    numItens--;
    printf("\n[OK] Item '%s' removido com sucesso!\n", nomeBusca);
}

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.
void listarItens() {
    printf("\n╔════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                        INVENTÁRIO DA MOCHILA                               ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════╝\n");

    if (numItens == 0) {
        printf("  Mochila vazia. Colete itens para sobreviver!\n");
        return;
    }

    printf("\n  Total de itens: %d/10\n\n", numItens);
    printf("┌────┬──────────────────────┬──────────────────┬────────────┬────────────┐\n");
    printf("│ Nº │ Nome                 │ Tipo             │ Quantidade │ Prioridade │\n");
    printf("├────┼──────────────────────┼──────────────────┼────────────┼────────────┤\n");

    for (int i = 0; i < numItens; i++) {
        printf("│ %2d │ %-20s │ %-16s │ %10d │ %10d │\n",
               i + 1,
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade,
               mochila[i].prioridade);
    }

    printf("└────┴──────────────────────┴──────────────────┴────────────┴────────────┘\n");
}

// insertionSort():
// Implementacao do algoritmo de ordenacao por insercao.
// Funciona com diferentes criterios de ordenacao:
// - Por nome (ordem alfabetica)
// - Por tipo (ordem alfabetica)
// - Por prioridade (da mais alta para a mais baixa)
void insertionSort(CriterioOrdenacao criterio) {
    comparacoes = 0;

    for (int i = 1; i < numItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;

        // Move elementos maiores que a chave uma posicao a frente
        while (j >= 0) {
            comparacoes++;
            bool deveMover = false;

            // Determina se deve mover baseado no critério
            switch (criterio) {
                case NOME:
                    deveMover = (strcmp(mochila[j].nome, chave.nome) > 0);
                    break;
                case TIPO:
                    deveMover = (strcmp(mochila[j].tipo, chave.tipo) > 0);
                    break;
                case PRIORIDADE:
                    // Ordem decrescente (maior prioridade primeiro)
                    deveMover = (mochila[j].prioridade < chave.prioridade);
                    break;
            }

            if (deveMover) {
                mochila[j + 1] = mochila[j];
                j--;
            } else {
                break;
            }
        }

        mochila[j + 1] = chave;
    }
}

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a funcao insertionSort() com o criterio selecionado.
// Exibe a quantidade de comparacoes feitas (analise de desempenho).
void menuDeOrdenacao() {
    if (numItens == 0) {
        printf("\n[AVISO] Mochila vazia! Nao ha itens para ordenar.\n");
        return;
    }

    int opcao;
    printf("\n┌─────────────────────────────────────┐\n");
    printf("│     CRITERIOS DE ORDENACAO          │\n");
    printf("└─────────────────────────────────────┘\n");
    printf("1. Ordenar por NOME (alfabetica)\n");
    printf("2. Ordenar por TIPO (alfabetica)\n");
    printf("3. Ordenar por PRIORIDADE (maior primeiro)\n");
    printf("0. Cancelar\n");
    printf("────────────────────────────────────\n");
    printf("Escolha o criterio: ");
    scanf("%d", &opcao);

    CriterioOrdenacao criterio;
    const char* nomeCriterio;

    switch (opcao) {
        case 1:
            criterio = NOME;
            nomeCriterio = "NOME";
            ordenadaPorNome = true;  // Marca que esta ordenada por nome
            break;
        case 2:
            criterio = TIPO;
            nomeCriterio = "TIPO";
            ordenadaPorNome = false;  // Nao esta ordenada por nome
            break;
        case 3:
            criterio = PRIORIDADE;
            nomeCriterio = "PRIORIDADE";
            ordenadaPorNome = false;  // Nao esta ordenada por nome
            break;
        case 0:
            return;
        default:
            printf("\n[ERRO] Opcao invalida!\n");
            return;
    }

    // Realiza a ordenacao
    insertionSort(criterio);

    printf("\n[OK] Itens ordenados por %s com sucesso!\n", nomeCriterio);
    printf("[DESEMPENHO] Analise de desempenho: %d comparacoes realizadas\n", comparacoes);

    // Exibe os itens ordenados
    listarItens();
}

// buscaBinariaPorNome():
// Realiza busca binaria por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrario, informa que nao encontrou o item.
void buscaBinariaPorNome() {
    if (numItens == 0) {
        printf("\n[AVISO] Mochila vazia! Nao ha itens para buscar.\n");
        return;
    }

    // Verifica se a mochila esta ordenada por nome
    if (!ordenadaPorNome) {
        printf("\n[ERRO] A busca binaria requer que a mochila esteja ordenada por NOME!\n");
        printf("       Por favor, ordene os itens por nome primeiro (opcao 4 do menu).\n");
        return;
    }

    char nomeBusca[30];
    printf("\n┌─────────────────────────────────────┐\n");
    printf("│     BUSCA BINARIA POR NOME          │\n");
    printf("└─────────────────────────────────────┘\n");
    printf("Nome do item a buscar: ");
    limparBuffer();
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    // Algoritmo de busca binaria
    int esquerda = 0;
    int direita = numItens - 1;
    int comparacoesBusca = 0;

    while (esquerda <= direita) {
        int meio = (esquerda + direita) / 2;
        comparacoesBusca++;

        int resultado = strcmp(mochila[meio].nome, nomeBusca);

        if (resultado == 0) {
            // Encontrou o item!
            printf("\n[OK] Item encontrado apos %d comparacoes!\n", comparacoesBusca);
            printf("\n┌─────────────────────────────────────┐\n");
            printf("│ Nome:       %-23s │\n", mochila[meio].nome);
            printf("│ Tipo:       %-23s │\n", mochila[meio].tipo);
            printf("│ Quantidade: %-23d │\n", mochila[meio].quantidade);
            printf("│ Prioridade: %-23d │\n", mochila[meio].prioridade);
            printf("└─────────────────────────────────────┘\n");
            printf("\n[INFO] Comparacoes na busca binaria: %d\n", comparacoesBusca);
            printf("       (Busca sequencial faria ate %d comparacoes)\n", numItens);
            return;
        } else if (resultado < 0) {
            // Item buscado esta na metade direita
            esquerda = meio + 1;
        } else {
            // Item buscado esta na metade esquerda
            direita = meio - 1;
        }
    }

    // Nao encontrou
    printf("\n[ERRO] Item '%s' nao encontrado apos %d comparacoes.\n", nomeBusca, comparacoesBusca);
}

int main() {
    // Menu principal com opcoes:
    // 1. Adicionar um item
    // 2. Remover um item
    // 3. Listar todos os itens
    // 4. Ordenar os itens por criterio (nome, tipo, prioridade)
    // 5. Realizar busca binaria por nome
    // 0. Sair

    int opcao;

    printf("\n");
    printf("════════════════════════════════════════════════════════════\n");
    printf("  BEM-VINDO AO DESAFIO CODIGO DA ILHA - EDICAO FREE FIRE\n");
    printf("  Nivel: MESTRE - Gerenciamento Avancado de Mochila\n");
    printf("════════════════════════════════════════════════════════════\n");
    printf("\n  Prepare sua mochila e organize os componentes para escapar!\n");

    // A estrutura switch trata cada opcao chamando a funcao correspondente.
    // A ordenacao e busca binaria exigem que os dados estejam bem organizados.

    do {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserirItem();
                break;

            case 2:
                removerItem();
                break;

            case 3:
                listarItens();
                break;

            case 4:
                menuDeOrdenacao();
                break;

            case 5:
                buscaBinariaPorNome();
                break;

            case 0:
                printf("\n");
                printf("════════════════════════════════════════════════════════════\n");
                printf("  Obrigado por jogar! Boa sorte na fuga da ilha!\n");
                printf("════════════════════════════════════════════════════════════\n");
                printf("\n");
                break;

            default:
                printf("\n[ERRO] Opcao invalida! Escolha entre 0 e 5.\n");
        }

        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            limparBuffer();
            getchar();
            limparTela();
        }

    } while (opcao != 0);

    return 0;
}
