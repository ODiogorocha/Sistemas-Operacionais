#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    __pid_t id_processo;

    if (id_processo < 0) {
        fprintf(stderr, "fork falhou\n");
    }else if (id_processo == 0){
        printf("Sou FIHO, id = %d, meu PAI eh %d\n\n", getpid(), getppid());
    }else{
        printf("Sou o PAI, id = %d, meu PAI eh %d\n\n", getpid(), getppid());

        getchar();
        exit(0);
    }
}