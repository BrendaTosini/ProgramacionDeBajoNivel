#include <stdio.h>

int main(){

    for (int i = 0; i < 127; i++){

        printf("%03d---%c \n",i,i);
    }
    return 0;
}