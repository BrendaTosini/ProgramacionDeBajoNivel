#include <stdio.h>

int main(){

    int numeros []= {2,7,3,56,9,23};
    int masAlto;
    for (int i=0; i < sizeof(numeros) / sizeof(int);i++){

        if (numeros[i] > masAlto){

            masAlto = numeros[i];
        }
    }
    printf("El numero mas alto es: %d \n", masAlto);
    return 0;
}