#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    __pid_t idProcesso;
    idProcesso = getppid();

    printf("\nPID =%d\n", idProcesso);
    printf("PPDI = %d\n\n", getppid());
    getchar();

    exit(0);
}