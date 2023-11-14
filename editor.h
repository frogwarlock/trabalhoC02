#ifndef EDITOR_H
#define EDITOR_H


//estrutura de FilmeInfo
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