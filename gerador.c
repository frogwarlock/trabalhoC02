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

void insereFilme(FilmeList* lista, int codigo, int ano, const char* nome_filme) {
    FilmeNo* novoFilme = malloc(sizeof(FilmeNo));
    novoFilme->codigo = codigo;
    novoFilme->ano = ano;
    snprintf(novoFilme->nome_filme, sizeof(novoFilme->nome_filme), "%s", nome_filme);
    novoFilme->next = NULL;

    if (lista->head == NULL) {
        lista->head = novoFilme;
    } else {
        FilmeNo* atual = lista->head;
        while (atual->next != NULL) {
            atual = atual->next;
        }
        atual->next = novoFilme;
    }
}

void ordenaFilmes(FilmeList* lista) {
    if (lista->head == NULL || lista->head->next == NULL) {
        return;  
    }

    FilmeNo* novaCabeca = NULL;
    FilmeNo* atual = lista->head;

    while (atual != NULL) {
        FilmeNo* proximo = atual->next;

        if (novaCabeca == NULL || atual->codigo < novaCabeca->codigo) {
            atual->next = novaCabeca;
            novaCabeca = atual;
        } else {
            FilmeNo* temp = novaCabeca;
            while (temp->next != NULL && temp->next->codigo < atual->codigo) {
                temp = temp->next;
            }
            atual->next = temp->next;
            temp->next = atual;
        }

        atual = proximo;
    }

    lista->head = novaCabeca;
}

void liberaLista(FilmeList* lista) {
    FilmeNo* atual = lista->head;
    while (atual != NULL) {
        FilmeNo* proximo = atual->next;
        free(atual);
        atual = proximo;
    }
}

void imprimeFilmes(FilmeNo* lista) {
    printf("Filmes ordenados por codigo:\n");
    while (lista != NULL) {
        printf("%d - %d - %s\n", lista->codigo, lista->ano, lista->nome_filme);
        lista = lista->next;
    }
}

void escreveBinario(FilmeList* lista, const char* nome_arquivo) {
    FILE* arquivo_binario = fopen(nome_arquivo, "wb");
    if (arquivo_binario == NULL) {
        printf("Um erro ocorreu ao abrir o arquivo %s para escrita\n", nome_arquivo);
        exit(1);
    }

    FilmeNo* atual = lista->head;
    while (atual != NULL) {
        fwrite(atual, sizeof(FilmeNo), 1, arquivo_binario);
        atual = atual->next;
    }

    fclose(arquivo_binario);
}

int gerador(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    FilmeList lista;
    lista.head = NULL;

    FILE *filmes_arquivo = fopen(argv[1], "r");
    if (filmes_arquivo == NULL) {
        printf("Um erro ocorreu ao abrir o arquivo %s\n", argv[1]);
        return 1; 
    }

    int codigo, ano;
    char nome_filme[50];

    while (fscanf(filmes_arquivo, "%d %d %[^\n]", &codigo, &ano, nome_filme) == 3) {
        insereFilme(&lista, codigo, ano, nome_filme);
    }

    fclose(filmes_arquivo);

    ordenaFilmes(&lista);

    imprimeFilmes(lista.head);

    escreveBinario(&lista, argv[2]);

    liberaLista(&lista);

    return 0;

}









/*#include <stdio.h>
#include <stdlib.h>

#define MAX_FILMES 100

typedef struct {
    int codigo;
    int ano;
    char nome_filme[50];
} FilmeInfo;

int comparaFilmesPorCodigo(const void *a, const void *b) {
    return ((FilmeInfo*)a)->codigo - ((FilmeInfo*)b)->codigo;
}



int main() {
    FilmeInfo *filmeInfo = malloc(MAX_FILMES * sizeof(FilmeInfo)); // ALOCAÇÃO DA MEMORIA
    int num_filme = 0;

    FILE *filmes_arquivo = fopen("filmes.txt", "r");
    if (filmes_arquivo == NULL) {
        printf("Um erro ocorreu ao abrir o arquivo filmes.txt");
        return 1; 
    }

    while (num_filme < MAX_FILMES && fscanf(filmes_arquivo, "%d %d %[^\n]", &filmeInfo[num_filme].codigo,
    &filmeInfo[num_filme].ano, filmeInfo[num_filme].nome_filme) == 3) {
        num_filme++;
    }

    fclose(filmes_arquivo);

    qsort(filmeInfo, num_filme, sizeof(FilmeInfo), comparaFilmesPorCodigo);

    FILE *arquivo_binario = fopen("filmesCrescente.bin", "wb");
    if (arquivo_binario == NULL) {
        printf("Um erro ocorreu ao abrir o arquivo filmes_ordenados.bin para escrita");
        free(filmeInfo);
        return 1;
    }

    fwrite(filmeInfo, sizeof(FilmeInfo), num_filme, arquivo_binario);

    fclose(arquivo_binario);

    //ordem dos filmes 
    printf("Filmes ordenados por código:\n");
    for (int i = 0; i < num_filme; i++) {
        printf("%d - %d - %s\n", filmeInfo[i].codigo, filmeInfo[i].ano, filmeInfo[i].nome_filme);
    }

    free(filmeInfo);

    return 0;
}
*/