// ifndef - Verificar se ele já foi definido
#ifndef GERADOR_H
#define GERADOR_H


// Estrutura usada no gerador.c
// Como se fosse escrever essas linhas direto no código

typedef struct FilmeNo {
    int codigo;
    int ano;
    char nome_filme[50];
    struct FilmeNo* next;
} FilmeNo;

typedef struct {
    FilmeNo* head;
} FilmeList;



#endif