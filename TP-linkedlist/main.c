
#include <string.h>
#include <malloc.h>
#include "linkedlist.h"
#include <stdio.h>


int main(){

    LinkedList *lista = crearLista();
    /*AgregarOrdenadamente(4,lista);
    char *resultado = ImprimirLista(lista);
    printf("La lista es: %s\n", resultado);

    AgregarOrdenadamente(3,lista);
    resultado = ImprimirLista(lista);
    printf("La lista es: %s\n", resultado);

    AgregarOrdenadamente(5,lista);
    resultado = ImprimirLista(lista);
    printf("La lista es: %s\n", resultado);
    */

    AgregarAlFinal(lista,4);
    AgregarAlPrincipio(lista,3);
    char *resultado = ImprimirLista(lista);
    printf("La lista es: %s\n", resultado);

    Eliminar(4,lista);
    char *re = ImprimirLista(lista);
    printf("Lista es: %s\n", re);

    /*int r = Buscar(4,lista);
    printf("Indice es: %d\n", r);

    int tamaño = TamañoLista(lista);
    printf("Tamaño es: %d\n", tamaño);

    AgregarEn(2,0,lista);
    char *imprimir = ImprimirLista(lista);
    printf("La lista es: %s\n", imprimir);
    */
    
    
    

    return 0;
}