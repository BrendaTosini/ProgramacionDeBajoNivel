#ifndef ESTUDIANTES_H
#define ESTUDIANTES_H

#include "linkedlist.h" 

typedef struct Estudiante {
    int id;
    char nombre[50];
    int edad;
    int cantidadNotas;
} Estudiante;

Estudiante* createEstudiante(const char *nombre, int edad);
bool equalsEstudiante(void *a, void *b);
void printEstudiante(void *value);
void agregarEstudiante(LinkedList *lista, const char *nombre, int edad);
void modificarEstudiante(LinkedList *lista, int id, const char *nombre, int edad);
void eliminarEstudiante(LinkedList *lista, int id);
void listarEstudiantes(LinkedList *lista);
Estudiante* buscarEstudiantePorNombre(LinkedList *lista, const char *nombre);
Estudiante* buscarEstudiantePorId(LinkedList *lista, int id);
LinkedList* buscarEstudiantesPorRangoDeEdad(LinkedList *lista, int edadMin, int edadMax);
int mostrarEstadisticasEstudiantes(LinkedList *lista);

#endif // ESTUDIANTES_H
