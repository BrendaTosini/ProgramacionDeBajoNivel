#ifndef SISTEMA_H
#define SISTEMA_H

#include "estudiantes.h"
#include "materias.h"
#include "linkedlist.h"
#include "notasMateria.h"

typedef struct Sistema {
    LinkedList *estudiantes;
    LinkedList *materias;
    LinkedList *notasMateria;
} Sistema;

void guardarEstudiantesCSV(Sistema *sistema, const char *filename);

void cargarEstudiantesCSV(Sistema *sistema, const char *filename);


void guardarMateriasCSV(Sistema *sistema, const char *filename);

void cargarMateriasCSV(Sistema *sistema, const char *filename);

void guardarNotasCSV(Sistema *sistema, const char *filename);

void cargarNotasCSV(Sistema *sistema, const char *filename);

void inicializarSistema(Sistema *sistema);

void generarEstudiantesDePrueba(Sistema *sistema, int cantidad);

void generarMateriasDePrueba(Sistema *sistema, int cantidad);

void generarNotasMateriaDePrueba(Sistema *sistema);

#endif // SISTEMA_H
