// No arquivo main.c

#include <stdio.h>
#include <stdlib.h>

// Inclua o código fonte do gerador
#include "gerador.h"

//Inclui o código do editor
#include "editor.h"

int main(int argc, char *argv[]) {
    gerador(argc, argv);
    editor(argc, argv);
    printf("Terminei o editor!");

}
