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

Node *criar_no(int dado, Node *pai) {
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

    if (pid_esquerda == 0) {
        raiz->esquerda = criar_no(raiz->dado * 2, raiz);
        exibir_processo("ARVORE");
        criar_processos_arvore(raiz->esquerda, nivel_atual + 1, altura_maxima);
        printf("[ÁRVORE] Processo %d finalizado.\n", getpid());
        exit(0);
    }

    if (pid_direita == 0) {
        raiz->direita = criar_no(raiz->dado * 2 + 1, raiz);
        exibir_processo("ARVORE");
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

void medir_tempo_arvore(Node *raiz_arvore, int altura) {
    struct timespec inicio_arvore, fim_arvore;
    clock_gettime(CLOCK_MONOTONIC, &inicio_arvore); //nesta linha o vscode trata o parametro CLOCK_MONOTONIC como erro mas dando um cntrl + click na biblioteca este parametro esta la 
    criar_processos_arvore(raiz_arvore, 0, altura);
    clock_gettime(CLOCK_MONOTONIC, &fim_arvore);
    double tempo_arvore = calcular_tempo_ms(inicio_arvore, fim_arvore);
    printf("Tempo total de criação da árvore: %.3f ms\n", tempo_arvore);
}

void medir_tempo_cadeia(Node *raiz_cadeia, int altura) {
    struct timespec inicio_cadeia, fim_cadeia;
    clock_gettime(CLOCK_MONOTONIC, &inicio_cadeia);
    criar_processos_cadeia(raiz_cadeia, 0, altura);
    clock_gettime(CLOCK_MONOTONIC, &fim_cadeia);
    double tempo_cadeia = calcular_tempo_ms(inicio_cadeia, fim_cadeia);
    printf("Tempo total de criação da cadeia: %.3f ms\n", tempo_cadeia);
}

int ler_altura(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <altura>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int altura = atoi(argv[1]);
    if (altura <= 0 || altura > ALTURA_MAXIMA) {
        fprintf(stderr, "Altura inválida. Use um valor entre 1 e %d.\n", ALTURA_MAXIMA);
        exit(EXIT_FAILURE);
    }

    return altura;
}

int main(int argc, char *argv[]) {
    int altura = ler_altura(argc, argv);
    int total_processos = (1 << (altura + 1)) - 1;

    printf("Altura da árvore: %d | Total de processos esperados: %d\n", altura, total_processos);

    Node *raiz_arvore = criar_no(1, NULL);
    Node *raiz_cadeia = criar_no(1, NULL);

    exibir_processo("RAIZ");

    medir_tempo_arvore(raiz_arvore, altura);
    medir_tempo_cadeia(raiz_cadeia, altura);

    return 0;
}
