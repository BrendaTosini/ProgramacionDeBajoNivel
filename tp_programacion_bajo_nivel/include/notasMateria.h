#include "materias.h"
#include "estudiantes.h"

typedef struct NotasMateria {

    int id_estudiante;
    int id_materia;
    int nota; 

}NotasMateria;


NotasMateria* createNotasMateria(int id_estudiante, int id_materia);
void inscribirEnMateria(LinkedList *lista_notas,int id_estudiante, int id_materia);
bool puedeAnotarse(LinkedList *lista_notas, int id_estudiante, int id_materia);
NotasMateria* buscarNotaInscripcion(LinkedList *lista_notas, int id_estudiante, int id_materia);
void rendirMateria(LinkedList *lista_notas,int nota,int id_estudiante, int id_materia);
LinkedList *listarNotasAprobadas(LinkedList *lista_notas, int id_estudiante);
LinkedList *listarNotasInscriptas(LinkedList *lista_notas, int id_estudiante);
LinkedList *listarNotasDesaprobadas(LinkedList *lista_notas, int id_estudiante);
int promedioDelEstudiante(LinkedList *lista_notas, int id_estudiante);
