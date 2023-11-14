#include <stdio.h>
#include <stdlib.h>

#include "editor.h"

// Mostra os filmes da lista filmes.txt no terminal
void exibirFilme(const FilmeNo* filme) {
    printf("%d - %d - %s\n", filme->codigo, filme->ano, filme->nome_filme);
}

// Mostra a lista feita em memoria heap
void exibirLista(const FilmeList* lista) {
    printf("Filmes ordenados por código:\n");
    FilmeNo* atual = lista->head;
    while (atual != NULL) {
        exibirFilme(atual);
        atual = atual->next;
    }
    printf("\n");
}

// Entrado o numero no terminal do filme ele busca no txt 
FilmeNo* buscarFilme(const FilmeList* lista, int chave) {
    FilmeNo* atual = lista->head;
    while (atual != NULL) {
        if (atual->codigo == chave) {
            return atual; // Retorna o no do filme encontrado
        }
        atual = atual->next;
    }
    return NULL; // Retorna NULL se o filme nao for encontrado
}

// Entrado o codigo, ano e nome ele insere o filme na lista
void inserirFilme(FilmeList* lista, int codigo, int ano, const char* nome_filme) {
    FilmeNo* novoFilme = malloc(sizeof(FilmeNo));
    if (novoFilme == NULL) {
        printf("Erro ao alocar memória para novo filme.\n");
        exit(1);
    }
    novoFilme->codigo = codigo;
    novoFilme->ano = ano;
    snprintf(novoFilme->nome_filme, sizeof(novoFilme->nome_filme), "%s", nome_filme);
    novoFilme->next = NULL;

    if (lista->head == NULL || codigo < lista->head->codigo) {
        novoFilme->next = lista->head;
        lista->head = novoFilme;
    } else {
        FilmeNo* atual = lista->head;
        while (atual->next != NULL && codigo > atual->next->codigo) {
            atual = atual->next;
        }
        novoFilme->next = atual->next;
        atual->next = novoFilme;
    }
}

// Remove o filme da lista em heap

void removerFilme(FilmeList* lista, int chave) {
    FilmeNo* atual = lista->head;
    if (atual == NULL) {
        printf("Lista vazia. Nenhum filme removido.\n"); // Caso a lista esteja vazia ele nao vai remover nenhum filme
        return;
    }

    // Caso especial: o nó a ser removido é o primeiro
    if (atual->codigo == chave) {
        lista->head = atual->next;
        free(atual);
        printf("Filme removido com sucesso.\n"); // Caso o codigo do filme encontrado ele remove o filme
        return;
    }

    // Busca o nó a ser removido
    while (atual->next != NULL && atual->next->codigo != chave) {
        atual = atual->next;
    }

    if (atual->next == NULL) {
        printf("Filme com código %d não encontrado.\n", chave); // Caso o codigo do filme nao for encontrado ele retorna essa mensagaem
        return;
    }

    // Remove o nó
    FilmeNo* temp = atual->next;
    atual->next = temp->next;
    free(temp);
    printf("Filme removido com sucesso.\n");
}


// Carrega o arquivo binario criado pelo gerador.c
void carregarArquivoBinario(FilmeList* lista, char* nome_arquivo) {
    FILE* arquivo_binario = fopen(nome_arquivo, "rb");
    if (arquivo_binario == NULL) {
        printf("Um erro ocorreu ao abrir o arquivo %s para leitura\n", nome_arquivo);
        exit(1);
    }

    FilmeNo temp;
    while (fread(&temp, sizeof(FilmeNo), 1, arquivo_binario) == 1) {
        inserirFilme(lista, temp.codigo, temp.ano, temp.nome_filme);
    }

    fclose(arquivo_binario);
}
// Salva o arquivo txt editado para binario
void salvarArquivoBinario(const FilmeList* lista, char* nome_arquivo) {
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

void liberarLista(FilmeList* lista) {
    FilmeNo* atual = lista->head;
    while (atual != NULL) {
        FilmeNo* proximo = atual->next;
        free(atual);
        atual = proximo;
    }
    lista->head = NULL; // Garante que a lista esta vazia apos liberar os nos
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_binario>\n", argv[0]);
        return 1;
    }

    FilmeList lista;
    lista.head = NULL;

    // Carregar filmes do arquivo binário
    carregarArquivoBinario(&lista, argv[1]);

    int opcao;
    do {
        printf("\nOpcoes:\n");
        printf("1. Inserir Filme\n");
        printf("2. Remover Filme\n");
        printf("3. Exibir Lista de Filmes\n");
        printf("4. Buscar Filme por Codigo\n");
        printf("5. Salvar Arquivo Antes\n");
        printf("0. Encerrar Programa\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) { // Entra na primeira opcao escolhida pelo usuario 
            case 1: {
                int codigo, ano;
                char nome_filme[50];
                printf("Digite o codigo do novo filme: ");
                scanf("%d", &codigo);
                printf("Digite o ano do novo filme: ");
                scanf("%d", &ano);
                printf("Digite o nome do novo filme: ");
                scanf(" %[^\n]", nome_filme);

                inserirFilme(&lista, codigo, ano, nome_filme);
                salvarArquivoBinario(&lista, argv[1]);

                break;
            }
            case 2: {
                int codigoRemover;
                printf("Digite o codigo do filme a ser removido: ");
                scanf("%d", &codigoRemover);

                removerFilme(&lista, codigoRemover);
                salvarArquivoBinario(&lista, argv[1]);

                break;
            }
            case 3:
                exibirLista(&lista);
                break;
            case 4: {
                int codigoBuscar;
                printf("Digite o codigo do filme a ser buscado: ");
                scanf("%d", &codigoBuscar);

                FilmeNo* filmeEncontrado = buscarFilme(&lista, codigoBuscar);
                if (filmeEncontrado != NULL) {
                    printf("Filme encontrado:\n");
                    printf("Codigo: %d\n", filmeEncontrado->codigo);
                    printf("Ano: %d\n", filmeEncontrado->ano);
                    printf("Nome: %s\n", filmeEncontrado->nome_filme);
                } else {
                    printf("Filme com codigo %d nao encontrado.\n", codigoBuscar);
                }

                break;
            }
            case 5:{
                char nomeBinario[50];
                printf("Digite o nome do arquivo (coloque .bin no fim): \n");

                scanf("%s", nomeBinario);

                salvarArquivoBinario(&lista, nomeBinario);
                break;
            }
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    liberarLista(&lista);

    return 0;
}