#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 5

sem_t mutex; // semaforo que controla a regiao critica 

void down(sem_t *S) {
    sem_wait(S);
}

void up(sem_t *S) {
    sem_post(S);
}

void desenhar_estado(int ativa_id, const char* estado) {
    system("clear");

    printf("=========== SEMÁFORO ===========\n\n");

    for (int i = 1; i <= NUM_THREADS; i++) {
        if (i == ativa_id) {
            printf("Thread %d --> [%s]  <==\n", i, estado);
        }else{
            printf("Thread %d --> [fora da região crítica]\n", i);
        }
    }

    printf("\n================================\n");
    fflush(stdout);
    usleep(700000);
}

void* executa_tarefa_thread(void *arg) {
    int id = *(int*)arg;

    desenhar_estado(id, "quer entrar");
    down(&mutex); //tenta entrar na regiao critica 

    desenhar_estado(id, "dentro");
    sleep(1); //simula a regiao critica 

    desenhar_estado(id, "saindo");
    up(&mutex); // sai da regiao critica 

    desenhar_estado(id, "fora");
    free(arg);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];

    // inicia o semaforo com valor 1 no mutex 
    sem_init(&mutex, 0, 1);

    for (int i = 0; i < NUM_THREADS; i++) {
        int* id = malloc(sizeof(int));
        *id = i +1;

        if (pthread_create(&threads[i], NULL, executa_tarefa_thread, id) != 0) {
            perror("Erro ao criar thread");
            return 1;
        }
    }

    //espera as threads terminarem
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&mutex); // destroi o semaforo
    printf("Todas as threads terminaram.\n");

    return 0;
}
