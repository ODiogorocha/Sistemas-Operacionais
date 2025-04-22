#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ALTURA_MAXIMA 10

typedef struct node {
    int dado;
    struct node *esquerda;
    struct node *direita;
    struct node *pai;
} Node;

Node *criar_no (int dado, Node *pai) {
    Node *novo = (Node *)malloc(sizeof(Node));
    novo->dado = dado;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->pai = pai;
    return novo;
}

void exibir_processo(const char *tipo) {
    printf("[%s] PID: %d | PPID: %d\n", tipo, getpid(), getppid());
}

void criar_processos_arvore(Node *raiz, int nivel_atual, int altura_maxima) {
    if (nivel_atual == altura_maxima) return;

    pid_t pid_esquerda = fork();
    pid_t pid_direita = fork();

    if(pid_esquerda == 0) {
        raiz->esquerda = criar_no(raiz->dado * 2, raiz);
        exibir_processo("ARVORE");
        criar_processos_arvore(raiz->esquerda, nivel_atual + 1, altura_maxima);
        printf("[ÁRVORE] Processo %d finalizado.\n", getpid());
        exit(0);
    }

    if (pid_direita == 0) {
        raiz->direita = criar_no(raiz->dado * 2 + 1, raiz);
        exibir_processo("ÁRVORE");
        criar_processos_arvore(raiz->direita, nivel_atual + 1, altura_maxima);
        printf("[ÁRVORE] Processo %d finalizado.\n", getpid());
        exit(0);
    }

    wait(NULL);
    wait(NULL);
}

void criar_processos_cadeia(Node *raiz, int nivel_atual, int altura_maxima) {
    if (nivel_atual == altura_maxima) return;

    pid_t pid = fork();

    if (pid == 0) {
        raiz->esquerda = criar_no(raiz->dado * 2, raiz);
        exibir_processo("CADEIA");
        criar_processos_cadeia(raiz->esquerda, nivel_atual + 1, altura_maxima);
        printf("[CADEIA] Processo %d finalizado.\n", getpid());
        exit(0);
    }

    wait(NULL);
}

double calcular_tempo_ms(struct timespec inicio, struct timespec fim) {
    return (fim.tv_sec - inicio.tv_sec) * 1000.0 + (fim.tv_nsec - inicio.tv_nsec) / 1e6;
}

int ler_altura(int argc, char *argv[]) {
    if (argc != 2) {
        printf(stderr, "Uso: %s <altura>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int altura = atoi(argv[1]);
    if (altura <= 0 || altura > ALTURA_MAXIMA) {
        printf(stderr, "Altura inválida. Use um valor entre 1 e %d.\n", ALTURA_MAXIMA);
        exit(EXIT_FAILURE);
    }

    return altura;
}

int main(int argc, char *argv[]) {
    int altura = ler_altura(argc, argv);
    int total_processos = (1 << (altura + 1)) - 1;

    printf("Altura da árvore: %d | Total de processos esperados: %d\n", altura, total_processos);

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    Node *raiz = criar_no(1, NULL);
    exibir_processo("RAIZ");

    // Chamada para a árvore ou cadeia
    criar_processos_arvore(raiz, 0, altura);  // Para árvore
    //criar_processos_cadeia(raiz, 0, altura);  // Para cadeia

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo_total = calcular_tempo_ms(inicio, fim);

    // Exibindo o tempo no formato esperado para captura pelo script
    printf("Tempo total de criação da árvore: %.3f ms\n", tempo_total);

    return 0;
}
