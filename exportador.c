#include <stdio.h>
#include <stdlib.h>

// Estrutura para o nó do filme
typedef struct FilmeNo {
    int codigo;
    int ano;
    char nome_filme[50];
    struct FilmeNo* next;
} FilmeNo;

// Estrutura para a lista de filmes
typedef struct FilmeList {
    FilmeNo* head;
} FilmeList;

// Função recursiva para inverter a lista
FilmeNo* inverterRecursivo(FilmeNo* atual, FilmeNo* anterior) {
    if (atual == NULL) {
        return anterior;
    }

    FilmeNo* proximo = atual->next;
    atual->next = anterior;

    return inverterRecursivo(proximo, atual);
}

// Função para inverter a lista usando a função recursiva
void inverterLista(FilmeList* lista_filmes) {
    lista_filmes->head = inverterRecursivo(lista_filmes->head, NULL);
}

// Função para exibir o filme
void exibirFilme(const FilmeNo* filme) {
    printf("%d - %d - %s\n", filme->codigo, filme->ano, filme->nome_filme);
}

// Função para exibir a lista de filmes
void exibirLista(const FilmeList* lista) {
    printf("Filmes ordenados por código:\n");
    FilmeNo* atual = lista->head;
    while (atual != NULL) {
        exibirFilme(atual);
        atual = atual->next;
    }
    printf("\n");
}

// Função principal do programa
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_binario> <arquivo_texto>\n", argv[0]);
        return 1;
    }

    FilmeList lista_filmes;
    lista_filmes.head = NULL;

    // Lê os dados do arquivo binário e constrói a lista encadeada
    FILE *arquivo_bin = fopen(argv[1], "rb");
    if (arquivo_bin == NULL) {
        printf("Erro ao abrir o arquivo binário %s para leitura\n", argv[1]);
        return EXIT_FAILURE;
    }

    FilmeNo *novo_filme;
    while ((novo_filme = (FilmeNo*)malloc(sizeof(FilmeNo))) != NULL &&
           fread(novo_filme, sizeof(FilmeNo), 1, arquivo_bin) == 1) {
        novo_filme->next = lista_filmes.head;
        lista_filmes.head = novo_filme;
    }

    fclose(arquivo_bin);

    // Inverte a lista usando a função recursiva
    inverterLista(&lista_filmes);

    // Abre o arquivo de texto para escrita
    FILE *arquivo_texto_ptr = fopen(argv[2], "w");
    if (arquivo_texto_ptr == NULL) {
        perror("Erro ao abrir o arquivo de texto para escrita");
        return EXIT_FAILURE;
    }

    // Escreve os dados no arquivo de texto
    FilmeNo *atual = lista_filmes.head;
    while (atual != NULL) {
        fprintf(arquivo_texto_ptr, "%d %d %s\n", atual->codigo, atual->ano, atual->nome_filme);
        atual = atual->next;
    }

    // Libera a memória alocada para cada nó da lista
    while (lista_filmes.head != NULL) {
        FilmeNo *temp = lista_filmes.head;
        lista_filmes.head = lista_filmes.head->next;
        free(temp);
    }

    fclose(arquivo_texto_ptr);

    printf("Exportacao concluida. Dados exportados para %s\n", argv[2]);

    return 0;
}
