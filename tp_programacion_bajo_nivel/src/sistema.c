#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

void guardarEstudiantesCSV(Sistema *sistema, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error al abrir el archivo para escribir");
        return;
    }
    Node *current = sistema->estudiantes->head;
    while (current != NULL) {
        Estudiante *est = (Estudiante*)current->value;
        fprintf(file, "%d,%s,%d\n", est->id, est->nombre, est->edad);
        current = current->next;
    }
    fclose(file);
}

void cargarEstudiantesCSV(Sistema *sistema, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir el archivo para leer");
        return;
    }
    char line[128];
    while (fgets(line, sizeof(line), file)) {
        char nombre[50];
        int id, edad;
        sscanf(line, "%d,%49[^,],%d", &id, nombre, &edad);
        Estudiante *est = createEstudiante(nombre, edad);
        est->id = id;
        append(sistema->estudiantes, est);
    }
    fclose(file);
}


void guardarMateriasCSV(Sistema *sistema, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error al abrir el archivo para escribir");
        return;
    }
    Node *current = sistema->materias->head;
    while (current != NULL) {
        Materia *materia = (Materia*)current->value;
        fprintf(file, "%d,%s\n", materia->id, materia->nombre);
        current = current->next;
    }
    fclose(file);
}

void cargarMateriasCSV(Sistema *sistema, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir el archivo para leer");
        return;
    }
    char line[128];
    while (fgets(line, sizeof(line), file)) {
        int id;
        char nombre[50];
        sscanf(line, "%d,%s", &id, nombre);
        Materia *materia = createMateria(nombre);
        materia->id = id;
        append(sistema->materias, materia);
    }
    fclose(file);
}

void guardarNotasCSV(Sistema *sistema, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error al abrir el archivo para escribir");
        return;
    }
    Node *current = sistema->notasMateria->head;
    while (current != NULL) {
        NotasMateria *nota_mat = (NotasMateria*)current->value;
        fprintf(file, "%d,%d,%d\n", nota_mat->id_estudiante, nota_mat->id_materia, nota_mat->nota);
        current = current->next;
    }
    fclose(file);
}

void cargarNotasCSV(Sistema *sistema, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir el archivo para leer");
        return;
    }
    char line[128];
    while (fgets(line, sizeof(line), file)) {
        int id_estudiante;
        int id_materia;
        int nota;
        sscanf(line, "%d,%d,%d", &id_estudiante, &id_materia, &nota);
        NotasMateria *nota_mat = createNotasMateria(id_estudiante,id_materia);
        nota_mat->nota = nota;
        append(sistema->notasMateria, nota_mat);
    }
    fclose(file);
}

void inicializarSistema(Sistema *sistema) {
    sistema->estudiantes = newLinkedList();
    sistema->materias = newLinkedList();
    sistema->notasMateria = newLinkedList();

    cargarEstudiantesCSV(sistema, "estudiantes.csv");
    cargarMateriasCSV(sistema, "materias.csv");
    cargarNotasCSV(sistema, "notasMateria.csv");
}

void generarEstudiantesDePrueba(Sistema *sistema, int cantidad) {
    char nombres[][10] = {"Juan", "Ana", "Luis", "Eva", "Leo"};
    for (int i = 0; i < cantidad; i++) {
        char nombre[50];
        sprintf(nombre, "%s%d", nombres[rand() % 5], i);
        int edad = rand() % 10 + 18;
        agregarEstudiante(sistema->estudiantes, nombre, edad);
    }
}

void generarMateriasDePrueba(Sistema *sistema, int cantidad) {
    char nombres[][15] = {"Matematica", "Fisica", "Quimica", "Historia", "Geografia"};
    for (int i = 0; i < cantidad; i++) {
        char nombre[50];
        sprintf(nombre, "%s%d", nombres[rand() % 5], i);
        agregarMateria(sistema->materias, nombre);
    }
}
