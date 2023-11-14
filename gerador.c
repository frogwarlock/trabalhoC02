#include <stdio.h>
#include <stdlib.h>

//Incluir o HEADER FILE como gerador.h
#include "gerador.h"


//alocação do nó, preenchendo código, ano e nome do filme e insere no final da lista
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

//Reorganiza os nós de forma ascendente com base nos códigos de cada filme
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

//percorre a lista nó por  para liberar a memória alocada
void liberaLista(FilmeList* lista) {
    FilmeNo* atual = lista->head;
    while (atual != NULL) {
        FilmeNo* proximo = atual->next;
        free(atual);
        atual = proximo;
    }
}

//imprime os filmes no terminal
void imprimeFilmes(FilmeNo* lista) {
    printf("\n");
    printf("Filmes ordenados por codigo:\n");
    while (lista != NULL) {
        printf("%d - %d - %s\n", lista->codigo, lista->ano, lista->nome_filme);
        lista = lista->next;
    }
}

   
void escreveBinario(FilmeList* lista, const char* nome_arquivo) {
    //cria um arquivo binário
    FILE* arquivo_binario = fopen(nome_arquivo, "wb");
    
    if (arquivo_binario == NULL) {
        printf("Um erro ocorreu ao abrir o arquivo %s para escrita\n", nome_arquivo);
        exit(1);
    }

    //percorre a lista  e escreve os dados 
    FilmeNo* atual = lista->head;
    while (atual != NULL) {
        fwrite(atual, sizeof(FilmeNo), 1, arquivo_binario);
        atual = atual->next;
    }

    fclose(arquivo_binario);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    FilmeList lista;
    lista.head = NULL;

    //abre o arquivo
    FILE *filmes_arquivo = fopen(argv[1], "r");
    if (filmes_arquivo == NULL) {
        printf("Um erro ocorreu ao abrir o arquivo %s\n", argv[1]);
        return 1; 
    }

    int codigo, ano;
    char nome_filme[50];

    //insere informações na lista com o nó
    while (fscanf(filmes_arquivo, "%d %d %[^\n]", &codigo, &ano, nome_filme) == 3) {
        insereFilme(&lista, codigo, ano, nome_filme);
    }
    //fecha
    fclose(filmes_arquivo);
    //ordena por código
    ordenaFilmes(&lista);
    //imprime na tela para visualização
    imprimeFilmes(lista.head);
    //escreve os dados em binário
    escreveBinario(&lista, argv[2]);
    //libera a memória alocada
    liberaLista(&lista);

    return 0;

}




