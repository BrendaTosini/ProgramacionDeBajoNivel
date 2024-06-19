#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "materias.h"



Materia* createMateria(const char *nombre) {
    Materia *nuevaMateria = (Materia*)malloc(sizeof(Materia));
    static int id_counter = 1;
    if (strcmp(nombre, "") == 0){
        return NULL;
    }
    nuevaMateria->id = id_counter++;
    strncpy(nuevaMateria->nombre, nombre, sizeof(nuevaMateria->nombre) - 1);
    nuevaMateria->nombre[sizeof(nuevaMateria->nombre) - 1] = '\0';
    return nuevaMateria;
}

bool equalsMateria(void *a, void *b) {
    Materia *est1 = (Materia*)a;
    Materia *est2 = (Materia*)b;
    return est1->id == est2->id;
}

void printMateria(void *value) {
    Materia *materia = (Materia*)value;
    printf("ID: %d, Nombre: %s\n", materia->id, materia->nombre);
}

void agregarMateria(LinkedList *lista, const char *nombre) {
    Materia *nuevaMateria = createMateria(nombre);
    append(lista, nuevaMateria);
}

void modificarMateria(LinkedList *lista, int id, const char *nombre) {
    Node *current = lista->head;
    while (current != NULL) {
        Materia *materia = (Materia*)current->value;
        if (materia->id == id) {
            strncpy(materia->nombre, nombre, sizeof(materia->nombre) - 1);
            materia->nombre[sizeof(materia->nombre) - 1] = '\0';
            return;
        }
        current = current->next;
    }
}

void eliminarMateria(LinkedList *lista, int id) {
    Materia temp;
    temp.id = id;
    removeNode(lista, &temp, equalsMateria);
}

void listarMaterias(LinkedList *lista) {
    printList(lista, printMateria);
}

Materia* buscarMateriaPorNombre(LinkedList *lista, const char *nombre) {
    Node *current = lista->head;
    while (current != NULL) {
        Materia *materia = (Materia*)current->value;
        if (strcmp(materia->nombre, nombre) == 0) {
            return materia;
        }
        current = current->next;
    }
    return NULL;
}

Materia* buscarMateriaPorId(LinkedList *lista, int id) {
    Node *current = lista->head;
    while (current != NULL) {
        Materia *materia = (Materia*)current->value;
        if (materia->id == id) {
            return materia;
        }
        current = current->next;
    }
    return NULL;
}

int mostrarEstadisticasMaterias(LinkedList *lista) {
    return lista->size;
}
