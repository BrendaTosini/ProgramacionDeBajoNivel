#include <stdio.h>

int main(){

    int numeros[3];
    int suma;

    printf("Ingrese 3 numeros: ");
    scanf("%d %d %d", &numeros[0],&numeros[1],&numeros[2]);

    for (int i = 0; i < sizeof(numeros)/ sizeof(int);i++){

        suma = suma + numeros[i];
    }

    float promedio = (float)suma / (sizeof(numeros) / sizeof(int));
    printf("El promedio es de: %f", promedio);
    
    return 0;
}