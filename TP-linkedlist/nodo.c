#include "nodo.h"
#include <malloc.h>

Nodo *crearNodo(int valor){
    Nodo *nodo = malloc(sizeof(Nodo));
    nodo->valor=valor;
    nodo->proximo = NULL;

    return nodo;

}