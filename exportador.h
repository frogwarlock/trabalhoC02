#ifndef EXPORTADOR_H
#define EXPORTADOR_H

// Estrutura usada no exportador 
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