#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    int pid = fork();

    if (pid != 0) {
        printf("Sou processo com PID = %d . meu pai eh %d, criei filho com PID = %d \n", getpid(), getppid(), pid);
    }else {
        sleep(10);
        printf("Sou filho com PID = %d , meu pai eh %d \n", getpid(), getppid());
    }
    printf("processo PID = %d, meu pai eh %d\n", getpid(), getppid());
}