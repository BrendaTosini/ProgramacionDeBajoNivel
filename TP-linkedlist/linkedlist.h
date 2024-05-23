#include "nodo.h"

typedef struct linkedList{
    Nodo *cabeza;
    Nodo *cola;
    int size;
}LinkedList;

LinkedList *crearLista();
int AgregarAlFinal(LinkedList *lista, int valor);
int AgregarAlPrincipio(LinkedList *lista, int valor);
int AgregarEn(int valor, int posicion, LinkedList *lista);
int AgregarOrdenadamente(int valor, LinkedList *lista);
int Buscar(int valor, LinkedList *lista);
int Eliminar(int valor, LinkedList *lista);
char *ImprimirLista(LinkedList *lista);
int TamañoLista(LinkedList *lista);