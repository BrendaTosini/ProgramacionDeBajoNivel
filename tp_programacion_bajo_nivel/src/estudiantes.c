#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estudiantes.h"

Estudiante* createEstudiante(const char *nombre, int edad) {
    Estudiante *nuevoEstudiante = (Estudiante*)malloc(sizeof(Estudiante));
    static int idCounter = 1;
    if (strcmp(nombre, "") == 0 || edad < 0){
        return NULL;
    }
    nuevoEstudiante->id = idCounter++;
    strncpy(nuevoEstudiante->nombre, nombre, sizeof(nuevoEstudiante->nombre) - 1);
    nuevoEstudiante->nombre[sizeof(nuevoEstudiante->nombre) - 1] = '\0';
    nuevoEstudiante->edad = edad;
    nuevoEstudiante->cantidadNotas = 0;

    return nuevoEstudiante;
}

bool equalsEstudiante(void *a, void *b) {
    Estudiante *est1 = (Estudiante*)a;
    Estudiante *est2 = (Estudiante*)b;
    return est1->id == est2->id;
}

void printEstudiante(void *value) {
    Estudiante *est = (Estudiante*)value;
    printf("ID: %d, Nombre: %s, Edad: %d\n", est->id, est->nombre, est->edad);
}

void agregarEstudiante(LinkedList *lista, const char *nombre, int edad) {
    Estudiante *nuevoEstudiante = createEstudiante(nombre, edad);
    append(lista, nuevoEstudiante);
}

void modificarEstudiante(LinkedList *lista, int id, const char *nombre, int edad) {
    Node *current = lista->head;
    while (current != NULL) {
        Estudiante *est = (Estudiante*)current->value;
        if (est->id == id) {
            strncpy(est->nombre, nombre, sizeof(est->nombre) - 1);
            est->nombre[sizeof(est->nombre) - 1] = '\0';
            est->edad = edad;
            return;
        }
        current = current->next;
    }
}

void eliminarEstudiante(LinkedList *lista, int id) {
    Estudiante temp;
    temp.id = id;
    removeNode(lista, &temp, equalsEstudiante);
}

void listarEstudiantes(LinkedList *lista) {
    printList(lista, printEstudiante);
}

Estudiante* buscarEstudiantePorNombre(LinkedList *lista, const char *nombre) {
    Node *current = lista->head;
    while (current != NULL) {
        Estudiante *est = (Estudiante*)current->value;
        if (strcmp(est->nombre, nombre) == 0) {
            return est;
        }
        current = current->next;
    }
    return NULL;
}

Estudiante* buscarEstudiantePorId(LinkedList *lista, int id) {
    Node *current = lista->head;
    while (current != NULL) {
        Estudiante *est = (Estudiante*)current->value;
        if (est->id == id) {
            return est;
        }
        current = current->next;
    }
    return NULL;
}

LinkedList *buscarEstudiantesPorRangoDeEdad(LinkedList *lista, int edadMin, int edadMax) {
    LinkedList *resultados = newLinkedList();
    Node *current = lista->head;
    while (current != NULL) {
        Estudiante *est = (Estudiante*)current->value;
        if (est->edad >= edadMin && est->edad <= edadMax) {
            append(resultados, est);
        }
        current = current->next;
    }
    return resultados;
}

int mostrarEstadisticasEstudiantes(LinkedList *lista) {
    int totalEdad = 0;
    int count = 0;
    Node *current = lista->head;
    while (current != NULL) {
        Estudiante *est = (Estudiante*)current->value;
        totalEdad += est->edad;
        count++;
        current = current->next;
    }
    if (count > 0) {
       return (double)totalEdad / count;
    } else {
        return 0;
    }
}
