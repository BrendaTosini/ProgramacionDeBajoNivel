#include <stdio.h>

int main(){

    int numeros[]= {5,83,32,84,3,6};
    int masBajo = numeros[0];

    for(int i=1; i < sizeof(numeros)/sizeof(int);i++){

        if (numeros[i] < masBajo){

            masBajo = numeros[i];
        }
    }

    printf("El numero mas bajo es: %d \n", masBajo);

    return 0;
}