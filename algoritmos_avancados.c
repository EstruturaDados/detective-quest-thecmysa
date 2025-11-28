#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ITENS 50
#define MAX_NOME 50

/*
   Sistema de Mochila – Versão Completa
   Feito por: Camilly Almeida

   Melhorias:
   - Categorias de itens
   - Ordenação alfabética
   - Busca inteligente
   - Remoção por nome ou número
   - Limite de peso da mochila
   - Salvar e carregar itens de arquivo
   - Listagem formatada
*/

typedef struct {
    char nome[MAX_NOME];
    char categoria[20];
    float peso;
} Item;

Item mochila[MAX_ITENS];
int qtd = 0;
float limitePeso = 30.0;    
float pesoAtual = 0.0;

// ------------------------ FUNÇÕES ------------------------

void adicionarItem() {
    if (qtd >= MAX_ITENS) {
        printf("\nA mochila está cheia!\n");
        return;
    }

    Item novo;

    printf("\n--- ADICIONAR ITEM ---\n");
    printf("Nome do item: ");
    fgets(novo.nome, MAX_NOME, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Categoria (Arma / Utilidade / Cura / Outro): ");
    fgets(novo.categoria, 20, stdin);
    novo.categoria[strcspn(novo.categoria, "\n")] = '\0';

    printf("Peso do item: ");
    scanf("%f", &novo.peso);
    getchar();

    if (pesoAtual + novo.peso > limitePeso) {
        printf("\nNão é possível adicionar! A mochila ficaria pesada demais.\n");
        return;
    }

    mochila[qtd++] = novo;
    pesoAtual += novo.peso;

    printf("\nItem adicionado com sucesso!\n");
}

void listarItens() {
    printf("\n========= ITENS NA MOCHILA =========\n");
    if (qtd == 0) {
        printf("A mochila está vazia.\n");
        return;
    }

    for (int i = 0; i < qtd; i++) {
        printf("%d. %s | Categoria: %s | Peso: %.2f kg\n",
               i + 1, mochila[i].nome, mochila[i].categoria, mochila[i].peso);
    }

    printf("\nPeso atual: %.2f kg / %.2f kg\n", pesoAtual, limitePeso);
}

void removerItem() {
    if (qtd == 0) {
        printf("- A mochila já está vazia.\n");
        return;
    }

    int escolha;
    printf("\nRemover por:\n1. Número\n2. Nome\nEscolha: ");
    scanf("%d", &escolha);
    getchar();

    if (escolha == 1) {
        int indice;
        printf("Número do item: ");
        scanf("%d", &indice);
        getchar();

        if (indice < 1 || indice > qtd) {
            printf("Número inválido.\n");
            return;
        }

        pesoAtual -= mochila[indice - 1].peso;

        for (int i = indice - 1; i < qtd - 1; i++)
            mochila[i] = mochila[i + 1];

        qtd--;
        printf("Item removido com sucesso!\n");

    } else if (escolha == 2) {
        char buscado[MAX_NOME];
        printf("Nome do item: ");
        fgets(buscado, MAX_NOME, stdin);
        buscado[strcspn(buscado, "\n")] = '\0';

        for (int i = 0; i < qtd; i++) {
            if (strcmp(mochila[i].nome, buscado) == 0) {
                pesoAtual -= mochila[i].peso;

                for (int j = i; j < qtd - 1; j++)
                    mochila[j] = mochila[j + 1];

                qtd--;
                printf("Item removido!\n");
                return;
            }
        }

        printf("Item não encontrado!\n");
    }
}

void buscarItem() {
    char buscado[MAX_NOME];
    printf("\nDigite o nome para buscar: ");
    fgets(buscado, MAX_NOME, stdin);
    buscado[strcspn(buscado, "\n")] = '\0';

    printf("\nResultados:\n");

    bool achou = false;
    for (int i = 0; i < qtd; i++) {
        if (strstr(mochila[i].nome, buscado) != NULL) {
            printf("%d. %s (%s) - %.2f kg\n", i + 1,
                   mochila[i].nome, mochila[i].categoria, mochila[i].peso);
            achou = true;
        }
    }

    if (!achou) printf("Nenhum item correspondente encontrado.\n");
}

void ordenarItens() {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = i + 1; j < qtd; j++) {
            if (strcmp(mochila[i].nome, mochila[j].nome) > 0) {
                Item temp = mochila[i];
                mochila[i] = mochila[j];
                mochila[j] = temp;
            }
        }
    }

    printf("\nItens ordenados alfabeticamente!\n");
}

void salvarArquivo() {
    FILE *f = fopen("mochila.txt", "w");

    for (int i = 0; i < qtd; i++) {
        fprintf(f, "%s;%s;%.2f\n",
                mochila[i].nome,
                mochila[i].categoria,
                mochila[i].peso);
    }

    fclose(f);
    printf("\nArquivo salvo com sucesso!\n");
}

void carregarArquivo() {
    FILE *f = fopen("mochila.txt", "r");
    if (!f) {
        printf("Nenhum arquivo encontrado.\n");
        return;
    }

    qtd = 0;
    pesoAtual = 0;

    while (!feof(f)) {
        Item item;
        if (fscanf(f, "%[^;];%[^;];%f\n",
                   item.nome, item.categoria, &item.peso) == 3) {
            mochila[qtd++] = item;
            pesoAtual += item.peso;
        }
    }

    fclose(f);
    printf("\nArquivo carregado!\n");
}

// ------------------------ MAIN ------------------------

int main() {
    int opcao;

    while (1) {
        printf("\n================ MENU DA MOCHILA ================\n");
        printf("1. Adicionar item\n");
        printf("2. Listar itens\n");
        printf("3. Remover item\n");
        printf("4. Buscar item\n");
        printf("5. Ordenar itens A-Z\n");
        printf("6. Salvar mochila em arquivo\n");
        printf("7. Carregar mochila do arquivo\n");
        printf("8. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: adicionarItem(); break;
            case 2: listarItens(); break;
            case 3: removerItem(); break;
            case 4: buscarItem(); break;
            case 5: ordenarItens(); break;
            case 6: salvarArquivo(); break;
            case 7: carregarArquivo(); break;
            case 8: 
                printf("\nSaindo... até logo!\n");
                return 0;
            default:
                printf("Opção inválida!\n");
        }
    }
}
