#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 5

sem_t mutex;  // Semáforo para controlar a região crítica

// Equivalente a down(S)
void down(sem_t *S) {
    sem_wait(S);
}

// Equivalente a up(S)
void up(sem_t *S) {
    sem_post(S);
}

// Função para desenhar o estado das threads na tela
void desenhar_estado(int ativa_id, const char* estado) {
    system("clear");  // No Windows use: system("cls");

    printf("=========== SEMÁFORO ===========\n\n");

    for (int i = 1; i <= NUM_THREADS; i++) {
        if (i == ativa_id) {
            printf("Thread %d --> [%s]  <==\n", i, estado);
        } else {
            printf("Thread %d --> [fora da região crítica]\n", i);
        }
    }

    printf("\n================================\n");
    fflush(stdout);
    usleep(700000);  // Aguarda 0.7 segundos para visualização
}

// Função executada por cada thread
void* tarefa(void* arg) {
    int id = *(int*)arg;

    desenhar_estado(id, "quer entrar");
    down(&mutex);  // Tenta entrar na região crítica

    desenhar_estado(id, "dentro");
    sleep(1);  // Simula trabalho na região crítica

    desenhar_estado(id, "saindo");
    up(&mutex);  // Sai da região crítica

    desenhar_estado(id, "fora");
    free(arg);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Inicializa o semáforo com valor 1 (mutex)
    sem_init(&mutex, 0, 1);

    for (int i = 0; i < NUM_THREADS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;

        if (pthread_create(&threads[i], NULL, tarefa, id) != 0) {
            perror("Erro ao criar thread");
            return 1;
        }
    }

    // Aguarda todas as threads terminarem
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&mutex);  // Destroi o semáforo
    printf("Todas as threads terminaram.\n");

    return 0;
}
