#include <stdio.h>
#include <stdlib.h>

typedef struct FilmeNo {
    int codigo;
    int ano;
    char nome_filme[50];
    struct FilmeNo* next;
} FilmeNo;

typedef struct {
    FilmeNo* head;
} FilmeList;

void inverterLista(FilmeList* lista_filmes) {
    FilmeNo *anterior = NULL, *atual = lista_filmes->head, *proximo;

    while (atual != NULL) {
        proximo = atual->next;
        atual->next = anterior;
        anterior = atual;
        atual = proximo;
    }

    lista_filmes->head = anterior;
}

void lerBinarioEscreverTexto(const char *arquivo_binario, const char *arquivo_texto) {
    FILE *arquivo_bin = fopen(arquivo_binario, "rb");
    if (arquivo_bin == NULL) {
        printf("Erro ao abrir o arquivo binário %s para leitura\n", arquivo_binario);
        exit(EXIT_FAILURE);
    }

    FilmeList lista_filmes;
    lista_filmes.head = NULL;

    // Lê os dados do arquivo binário e constrói a lista encadeada
    FilmeNo *novo_filme;
    while ((novo_filme = (FilmeNo*)malloc(sizeof(FilmeNo))) != NULL &&
           fread(novo_filme, sizeof(FilmeNo), 1, arquivo_bin) == 1) {
        novo_filme->next = lista_filmes.head;
        lista_filmes.head = novo_filme;
    }

    fclose(arquivo_bin);

    // Inverte a lista para ter a ordem crescente
    inverterLista(&lista_filmes);

    // Abre o arquivo de texto para escrita
    FILE *arquivo_texto_ptr = fopen(arquivo_texto, "w");
    if (arquivo_texto_ptr == NULL) {
        perror("Erro ao abrir o arquivo de texto para escrita");
        exit(EXIT_FAILURE);
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

    printf("Exportacao concluida. Dados exportados para %s\n", arquivo_texto);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_binario> <arquivo_texto>\n", argv[0]);
        return 1;
    }

    lerBinarioEscreverTexto(argv[1], argv[2]);

    return 0;
}
