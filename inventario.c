/*
 * SISTEMA DE INVENTÁRIO - FREE FIRE (NÍVEL NOVATO)
 *
 * Descrição: Sistema de gerenciamento de inventário (mochila) que permite
 * ao jogador coletar, organizar e gerenciar itens do jogo (armas, munições, kits).
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Definição de constantes
#define MAX_ITENS 10
#define TAM_NOME 30
#define TAM_TIPO 20

/*
 * Struct Item
 * Representa um item do inventário com suas propriedades essenciais.
 *
 * Campos:
 *   - nome: identificador do item (ex: "AK-47", "Kit Médico")
 *   - tipo: categoria do item (arma, munição, cura, ferramenta)
 *   - quantidade: número de unidades deste item na mochila
 */
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
} Item;

// Vetor global de itens (inventário) e contador de itens ativos
Item inventario[MAX_ITENS];
int totalItens = 0;

/*
 * Função: limparBuffer
 * Limpa o buffer do teclado para evitar problemas com leitura de strings
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * Função: inserirItem
 * Adiciona um novo item ao inventário do jogador.
 *
 * Funcionamento:
 *   1. Verifica se há espaço disponível (máximo 10 itens)
 *   2. Solicita os dados do item ao usuário
 *   3. Armazena o item no próximo espaço livre do vetor
 *   4. Incrementa o contador de itens
 *
 * Complexidade: O(1) - inserção no final do vetor
 */
void inserirItem() {
    // Validação: verifica se o inventário está cheio
    if (totalItens >= MAX_ITENS) {
        printf("\n❌ Mochila cheia! Capacidade máxima atingida (%d itens).\n", MAX_ITENS);
        return;
    }

    printf("\n┌─────────────────────────────────────┐\n");
    printf("│   CADASTRAR NOVO ITEM NO INVENTÁRIO   │\n");
    printf("└───────────────────────────────────────┘\n");

    Item novoItem;

    // Leitura do nome do item
    printf("Digite o nome do item: ");
    limparBuffer();
    fgets(novoItem.nome, TAM_NOME, stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = '\0'; // Remove o '\n' do final

    // Leitura do tipo do item
    printf("Digite o tipo (arma/munição/cura/ferramenta): ");
    fgets(novoItem.tipo, TAM_TIPO, stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = '\0';

    // Leitura da quantidade
    printf("Digite a quantidade: ");
    scanf("%d", &novoItem.quantidade);

    // Validação da quantidade
    if (novoItem.quantidade <= 0) {
        printf(" Quantidade inválida! Deve ser maior que zero.\n");
        return;
    }

    // Armazena o item no vetor de inventário
    inventario[totalItens] = novoItem;
    totalItens++;

    printf("\n Item '%s' cadastrado com sucesso!\n", novoItem.nome);
}

/*
 * Função: removerItem
 * Remove um item específico do inventário usando seu nome.
 *
 * Funcionamento:
 *   1. Busca o item pelo nome usando busca sequencial
 *   2. Se encontrado, move todos os itens posteriores uma posição para trás
 *   3. Decrementa o contador de itens
 *
 * Complexidade: O(n) - busca + reorganização do vetor
 */
void removerItem() {
    if (totalItens == 0) {
        printf("\n Inventário vazio! Não há itens para remover.\n");
        return;
    }

    char nomeBusca[TAM_NOME];
    printf("\n┌─────────────────────────────────────┐\n");
    printf("│      REMOVER ITEM DO INVENTÁRIO      │\n");
    printf("└─────────────────────────────────────┘\n");
    printf("Digite o nome do item a remover: ");
    limparBuffer();
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    // Busca sequencial pelo item
    int encontrado = -1;
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(inventario[i].nome, nomeBusca) == 0) {
            encontrado = i;
            break;
        }
    }

    // Se o item não foi encontrado
    if (encontrado == -1) {
        printf(" Item '%s' não encontrado no inventário.\n", nomeBusca);
        return;
    }

    // Remove o item movendo todos os posteriores uma posição para trás
    // Isso mantém o vetor sem "buracos" (compactado)
    for (int i = encontrado; i < totalItens - 1; i++) {
        inventario[i] = inventario[i + 1];
    }

    totalItens--;
    printf(" Item '%s' removido com sucesso!\n", nomeBusca);
}

/*
 * Função: listarItens
 * Exibe todos os itens atualmente armazenados no inventário.
 *
 * Funcionamento:
 *   - Percorre o vetor de itens de 0 até totalItens-1
 *   - Formata e exibe as informações de cada item
 *
 * Complexidade: O(n) - percorre todos os itens
 */
void listarItens() {
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    INVENTÁRIO ATUAL                          ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");

    if (totalItens == 0) {
        printf("   Inventário vazio. Colete itens para começar!\n");
        return;
    }

    printf("  Total de itens: %d/%d\n\n", totalItens, MAX_ITENS);
    printf("┌────┬──────────────────────┬──────────────────┬────────────┐\n");
    printf("│ Nº │ Nome                 │ Tipo             │ Quantidade │\n");
    printf("├────┼──────────────────────┼──────────────────┼────────────┤\n");

    // Percorre e exibe cada item do inventário
    for (int i = 0; i < totalItens; i++) {
        printf("│ %2d │ %-20s │ %-16s │ %10d │\n",
               i + 1,
               inventario[i].nome,
               inventario[i].tipo,
               inventario[i].quantidade);
    }

    printf("└────┴──────────────────────┴──────────────────┴────────────┘\n");
}

/*
 * Função: buscarItem
 * Realiza uma busca sequencial por um item específico no inventário.
 *
 * Funcionamento:
 *   1. Solicita o nome do item a buscar
 *   2. Percorre o vetor comparando cada nome
 *   3. Se encontrado, exibe os dados completos do item
 *
 * Complexidade: O(n) - busca linear no pior caso
 * Nota: Para grandes volumes, uma busca binária seria mais eficiente (O(log n)),
 *       mas exigiria que o vetor estivesse ordenado.
 */
void buscarItem() {
    if (totalItens == 0) {
        printf("\n Inventário vazio! Não há itens para buscar.\n");
        return;
    }

    char nomeBusca[TAM_NOME];
    printf("\n┌─────────────────────────────────────┐\n");
    printf("│       BUSCAR ITEM NO INVENTÁRIO      │\n");
    printf("└─────────────────────────────────────┘\n");
    printf("Digite o nome do item: ");
    limparBuffer();
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    // Busca sequencial
    int encontrado = -1;
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(inventario[i].nome, nomeBusca) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("\n Item '%s' não encontrado no inventário.\n", nomeBusca);
    } else {
        printf("\n Item encontrado!\n");
        printf("┌─────────────────────────────────────┐\n");
        printf("│ Nome:       %-23s │\n", inventario[encontrado].nome);
        printf("│ Tipo:       %-23s │\n", inventario[encontrado].tipo);
        printf("│ Quantidade: %-23d │\n", inventario[encontrado].quantidade);
        printf("└─────────────────────────────────────┘\n");
    }
}

/*
 * Função: exibirMenu
 * Exibe o menu principal do sistema de inventário
 */
void exibirMenu() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║             SISTEMA DE INVENTÁRIO - FREE FIRE              ║\n");
    printf("║                    NÍVEL NOVATO                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("  1. Cadastrar item\n");
    printf("  2. Remover item\n");
    printf("  3. Listar todos os itens\n");
    printf("  4. Buscar item\n");
    printf("  5. Sair\n");
    printf("────────────────────────────────────────────────────────────\n");
    printf("  Escolha uma opção: ");
}

/*
 * Função: main
 * Função principal que controla o fluxo do programa.
 *
 * Estrutura:
 *   - Loop infinito que exibe o menu
 *   - Switch-case para processar a escolha do usuário
 *   - Chama as funções apropriadas para cada operação
 *   - Atualiza e exibe o inventário após cada operação
 */
int main() {
    int opcao;

    // Mensagem de boas-vindas
    printf("\n");
    printf("════════════════════════════════════════════════════════════\n");
    printf("  Bem-vindo ao Sistema de Gerenciamento de Inventário!\n");
    printf("  Organize sua mochila e maximize sua estratégia de loot!\n");
    printf("════════════════════════════════════════════════════════════\n");

    // Loop principal do programa
    while (1) {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserirItem();
                listarItens(); // Mostra o inventário atualizado
                break;

            case 2:
                removerItem();
                listarItens(); // Mostra o inventário atualizado
                break;

            case 3:
                listarItens();
                break;

            case 4:
                buscarItem();
                break;

            case 5:
                printf("\n Encerrando sistema... Boa sorte no campo de batalha!\n\n");
                return 0;

            default:
                printf("\n Opção inválida! Escolha entre 1 e 5.\n");
        }

        // Pausa para o usuário visualizar o resultado
        printf("\nPressione ENTER para continuar...");
        limparBuffer();
        getchar();

        // Limpa a tela (funciona em Linux/Mac)
        // Para Windows, use: system("cls");
        system("clear");
    }

    return 0;
}
