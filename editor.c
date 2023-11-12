#include <stdio.h>
#include <stdlib.h>

//ligação com o editor.h
#include "editor.h" 

#define MAX_FILMES 100

int comparaFilmesPorCodigo(const void *a, const void *b) {
    return ((FilmeInfo *)a)->codigo - ((FilmeInfo *)b)->codigo;
}

void exibirFilme(const FilmeInfo *filme) {
    printf("%d - %d - %s\n", filme->codigo, filme->ano, filme->nome_filme);
}

void exibirLista(const FilmeInfo *lista, int tamanho) {
    printf("Lista de Filmes:\n");
    for (int i = 0; i < tamanho; i++) {
        exibirFilme(&lista[i]);
    }
    printf("\n");
}

int buscarFilme(const FilmeInfo *lista, int tamanho, int chave) {
    for (int i = 0; i < tamanho; i++) {
        if (lista[i].codigo == chave) {
            return i; // Retorna o índice do filme encontrado
        }
    }
    return -1; // Retorna -1 se o filme não for encontrado
}

void inserirFilme(FilmeInfo *lista, int *tamanho, FilmeInfo novoFilme) {
    // Adiciona o novo filme no final da lista
    lista[*tamanho] = novoFilme;
    (*tamanho)++;
    // Reordena a lista com base no código após a inserção
    qsort(lista, *tamanho, sizeof(FilmeInfo), comparaFilmesPorCodigo);
}

void removerFilme(FilmeInfo *lista, int *tamanho, int chave) {
    int indice = buscarFilme(lista, *tamanho, chave);
    if (indice != -1) {
        // Move os filmes após o filme removido para ocupar o espaço
        for (int i = indice; i < *tamanho - 1; i++) {
            lista[i] = lista[i + 1];
        }
        (*tamanho)--;
    } else {
        printf("Filme com código %d não encontrado.\n", chave);
    }
}

void carregarArquivoBinario(FilmeInfo *filmeInfo, int *num_filme, char *argv[]) {
    FILE *arquivo = fopen(argv[1], "rb");
    if (arquivo != NULL) {
        fread(filmeInfo, sizeof(FilmeInfo), MAX_FILMES, arquivo);
        fclose(arquivo);
        *num_filme = MAX_FILMES; // Atualiza o número de filmes com base na leitura
    } else {
        printf("Um erro ocorreu ao abrir o arquivo %s para leitura. Certifique-se de executar o gerador.c primeiro.\n", argv[1]);
        exit(1);
    }
}

void salvarArquivoBinario(const FilmeInfo *filmeInfo, int num_filme, char *argv[]) {
    FILE *arquivo = fopen(argv[1], "wb");
    if (arquivo != NULL) {
        fwrite(filmeInfo, sizeof(FilmeInfo), num_filme, arquivo);
        fclose(arquivo);
    } else {
        printf("Um erro ocorreu ao abrir o arquivo %s para escrita\n", argv[1]);
    }
}

int editor(int argc, char *argv[]) {
    FilmeInfo *filmeInfo = malloc(MAX_FILMES * sizeof(FilmeInfo)); // ALOCAÇÃO DA MEMORIA
    int num_filme = 0;

    carregarArquivoBinario(filmeInfo, &num_filme, argv);

    int opcao;
    do {
        printf("\nOpcoes:\n");
        printf("1. Inserir Filme\n");
        printf("2. Remover Filme\n");
        printf("3. Exibir Lista de Filmes\n");
        printf("4. Buscar Filme por Codigo\n");
        printf("0. Encerrar Programa\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                FilmeInfo novoFilme;
                printf("Digite o codigo do novo filme: ");
                scanf("%d", &novoFilme.codigo);
                printf("Digite o ano do novo filme: ");
                scanf("%d", &novoFilme.ano);
                printf("Digite o nome do novo filme: ");
                scanf(" %[^\n]", novoFilme.nome_filme);

                inserirFilme(filmeInfo, &num_filme, novoFilme);
                break;
            }
            case 2: {
                int codigoRemover;
                printf("Digite o codigo do filme a ser removido: ");
                scanf("%d", &codigoRemover);

                removerFilme(filmeInfo, &num_filme, codigoRemover);
                break;
            }
            case 3:
                exibirLista(filmeInfo, num_filme);
                break;
            case 4: {
                int codigoBuscar;
                printf("Digite o codigo do filme a ser buscado: ");
                scanf("%d", &codigoBuscar);

                int indice = buscarFilme(filmeInfo, num_filme, codigoBuscar);
                if (indice != -1) {
                    printf("Filme encontrado:\n");
                    exibirFilme(&filmeInfo[indice]);
                } else {
                    printf("Filme com codigo %d nao encontrado.\n", codigoBuscar);
                }
                break;
            }
            case 0:
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    salvarArquivoBinario(filmeInfo, num_filme, argv);

    free(filmeInfo);

    return 0;
}
