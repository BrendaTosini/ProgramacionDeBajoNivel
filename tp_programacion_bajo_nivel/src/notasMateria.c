#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "notasMateria.h"

NotasMateria* createNotasMateria(int id_estudiante, int id_materia) {
    NotasMateria *nuevoNotasMateria = (NotasMateria*)malloc(sizeof(NotasMateria));
    nuevoNotasMateria->id_estudiante = id_estudiante;
    nuevoNotasMateria->id_materia = id_materia;
    nuevoNotasMateria->nota = 0;

    return nuevoNotasMateria;
}

void inscribirEnMateria(LinkedList *lista_notas,int id_estudiante, int id_materia) {
    NotasMateria *nota_mat = createNotasMateria(id_estudiante,id_materia);
    append(lista_notas,nota_mat);
}

bool puedeAnotarse(LinkedList *lista_notas, int id_estudiante, int id_materia) {
    Node *current = lista_notas->head;
    while (current != NULL) {
        NotasMateria *nota_mat= (NotasMateria*)current->value;
        if (nota_mat->id_estudiante == id_estudiante && nota_mat->id_materia == id_materia && (nota_mat->nota >= 4 || nota_mat->nota == 0)) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

NotasMateria* buscarNotaInscripcion(LinkedList *lista_notas, int id_estudiante, int id_materia) {
    Node *current = lista_notas->head;
    while (current != NULL) {
        NotasMateria *nota_mat= (NotasMateria*)current->value;
        if (nota_mat->id_estudiante == id_estudiante && nota_mat->id_materia == id_materia && nota_mat->nota == 0) {
            return nota_mat;
        }
        current = current->next;
    }
    return NULL;
}

void rendirMateria(LinkedList *lista_notas,int nota,int id_estudiante, int id_materia) {
    NotasMateria *nota_mat = buscarNotaInscripcion(lista_notas,id_estudiante,id_materia);
    nota_mat->nota = nota;
}

LinkedList *listarNotasAprobadas(LinkedList *lista_notas, int id_estudiante) {
    LinkedList *resultados = newLinkedList();
    Node *current = lista_notas->head;
    while (current != NULL) {
        NotasMateria *nota_mat= (NotasMateria*)current->value;
        if (nota_mat->id_estudiante == id_estudiante  && nota_mat->nota >= 4){
            append(resultados,nota_mat);
            
        }
        current = current->next; 
    }
    return resultados;
}

LinkedList *listarNotasInscriptas(LinkedList *lista_notas, int id_estudiante) {
    LinkedList *resultados = newLinkedList();
    Node *current = lista_notas->head;
    while (current != NULL) {
        NotasMateria *nota_mat= (NotasMateria*)current->value;
        if (nota_mat->id_estudiante == id_estudiante  && nota_mat->nota == 0){
            append(resultados,nota_mat);
            
        }
        current = current->next; 
    }
    return resultados;
}

LinkedList *listarNotasDesaprobadas(LinkedList *lista_notas, int id_estudiante) {
    LinkedList *resultados = newLinkedList();
    Node *current = lista_notas->head;
    while (current != NULL) {
        NotasMateria *nota_mat= (NotasMateria*)current->value;
        if (nota_mat->id_estudiante == id_estudiante  && nota_mat->nota > 0 && nota_mat->nota < 4){
            append(resultados,nota_mat);
            
        }
        current = current->next; 
    }
    return resultados;
}

int promedioDelEstudiante(LinkedList *lista_notas, int id_estudiante){
   Node *current = lista_notas->head;
    int nota_total = 0;
    float promedio = 0;
    int contador = 0;
    while (current != NULL) {
        NotasMateria *nota_mat= (NotasMateria*)current->value;
        if (id_estudiante == nota_mat->id_estudiante && nota_mat->nota >= 4){
            nota_total = nota_total + nota_mat->nota;
            contador = contador +1;
        }
        current = current->next; 
    }
    if (nota_total == 0){
        promedio = 0;
    }else{
        promedio  = nota_total/contador;
    }
    return promedio;
}

