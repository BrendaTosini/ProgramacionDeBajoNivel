#ifndef MATERIAS_H
#define MATERIAS_H

#include "linkedlist.h" 

typedef struct Materia {
    int id;
    char nombre[50];
} Materia;

Materia* createMateria(const char *nombre);
bool equalsMateria(void *a, void *b);
void printMateria(void *value);
void agregarMateria(LinkedList *lista, const char *nombre);
void modificarMateria(LinkedList *lista, int id, const char *nombre);
void eliminarMateria(LinkedList *lista, int id);
void listarMaterias(LinkedList *lista);
Materia* buscarMateriaPorNombre(LinkedList *lista, const char *nombre);
Materia* buscarMateriaPorId(LinkedList *lista, int id);
int mostrarEstadisticasMaterias(LinkedList *lista);

#endif // MATERIAS_H
