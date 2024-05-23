#include "linkedlist.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>


LinkedList *crearLista(){
    Nodo *cabeza =malloc(sizeof(Nodo));
    Nodo *cola = malloc(sizeof(Nodo));
    cabeza = NULL;
    cola =NULL;

    LinkedList *lista = malloc(sizeof(LinkedList));
    lista->cabeza=cabeza;
    lista->cola=cola;
    lista->size = 0;

    return lista;
}

int AgregarAlFinal(LinkedList *lista, int valor){

    lista->size++;
    Nodo *nodo = crearNodo(valor);
    
    if (lista->cabeza == NULL){

        lista->cabeza = nodo;
        lista->cola = nodo;
        return 0;

    }
    lista->cola->proximo = nodo;
    lista->cola = nodo;

    return 0;
    
}

int AgregarAlPrincipio(LinkedList *lista, int valor){

    lista->size++;
    Nodo *nodo = crearNodo(valor);

    if (lista->cabeza == NULL){

        lista->cabeza = nodo;
        lista->cola = nodo;

        return 0;
    }

    nodo->proximo = lista->cabeza;
    lista->cabeza = nodo;

}

int AgregarEn(int valor, int posicion, LinkedList *lista){

    if (posicion < 0){
        return -1;
    }

    if (posicion == 0){

        AgregarAlPrincipio(lista,valor);

        return 0;
    }

    Nodo *nodoActual = lista->cabeza;

    while (nodoActual != NULL && posicion > 1){

        nodoActual = nodoActual->proximo;
        posicion--;

    }

    if (nodoActual == NULL){

        return 0;
    }

    Nodo *nodo = crearNodo(valor);
    nodo->proximo = nodoActual->proximo;
    nodoActual->proximo = nodo;
    lista->size++;
    

    return 0;
}

int AgregarOrdenadamente(int valor, LinkedList *lista){

    lista->size++;
    Nodo *nodo = crearNodo(valor);
    
    if (lista->cabeza == NULL){

        lista->cabeza = nodo;
        lista->cola = nodo;
        return 0;

    }
    
    Nodo *nodoActual = lista->cabeza;

    while (nodoActual !=NULL){

        if (nodo->valor <= nodoActual->valor){
            nodo->proximo = nodoActual;
            lista->cabeza = nodo;
            return 0;
        }

        if (nodo->valor > nodoActual->valor){

            if (nodoActual->proximo == NULL){

                nodoActual->proximo = nodo;
                return 0;
            }

            if (nodo->valor < nodoActual->proximo->valor){

                nodo->proximo = nodoActual->proximo;
                nodoActual->proximo = nodo;
                return 0;

            }
            nodoActual = nodoActual->proximo;
           
        } 
        

    }


}

int Buscar(int valor, LinkedList *lista){

    if (lista->cabeza == NULL){

        return -1;        
    }

    Nodo *nodoActual = lista->cabeza;
    int posicion = 0;

    while (nodoActual != NULL){

        if (nodoActual->valor == valor){

            return posicion;
        }
        nodoActual = nodoActual->proximo;
        posicion++;

    }
    return -1;

}

int Eliminar(int valor, LinkedList *lista){

    if (lista->cabeza == NULL){

        return -1;
    }
    lista->size--;

    Nodo *temporal = lista->cabeza;

    if (lista->cabeza->valor == valor){

        lista->cabeza = lista->cabeza->proximo;
        free(temporal);
        return 0;
    }

    Nodo *nodoActual = lista->cabeza;

    while (nodoActual->proximo != NULL){

        if (nodoActual->proximo->valor == valor){

            temporal =nodoActual->proximo;
            nodoActual->proximo = nodoActual->proximo->proximo;
            free(temporal);

            return 0;
        }
        nodoActual = nodoActual->proximo;
    } 
}

char *ImprimirLista(LinkedList *lista){

    if (lista->cabeza == NULL){
        char *cadena = "[]";
        return cadena;
    }

    Nodo *nodoActual = lista->cabeza;

    char *resultado= malloc(20 * sizeof(char));
    resultado[0] = '\0';
    strcat(resultado, "[");

    while (nodoActual != NULL){

        char numeroAstring[10];
        sprintf(numeroAstring, "%d" ,nodoActual->valor);
        strcat(resultado,numeroAstring);
        if (nodoActual->proximo != NULL){

            strcat(resultado,",");
        }
        nodoActual = nodoActual->proximo;
    }
    strcat(resultado, "]");
    
    

    return resultado;

}

int TamañoLista(LinkedList *lista){

    if (lista->cabeza == NULL){

        return 0;
    }

    return lista->size;
}