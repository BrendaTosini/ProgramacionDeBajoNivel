#include <stdio.h>

int main(){

    char nombre[10];
    int edad;
    printf("Ingrese su nombre: ");
    scanf("%s", nombre);
    printf("Ingrese su edad: ");
    scanf("%d", &edad);
    printf("Su nombre es: %s \n", nombre);
    printf("Su edad es: %d \n", edad);
    return 0;

}