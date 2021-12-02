#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "timer.h"
#include <unistd.h>
#include <pthread.h>

#define COLOR_BOLD_BLUE "\e[1;34m"
#define COLOR_BOLD_RED "\e[1;31m"
#define COLOR_BOLD_YELLOW "\e[1;33m"
#define COLOR_RESET "\x1b[0m"

long long int n;
float inferior, superior, *vetor;
int n_threads;

void preenche_vetor(float *vet) {
    srand(time(NULL));
    for (int i = 0 ; i < n ; i++)
        vet[i] = (float) rand() / (float) RAND_MAX * 100; // 0 <= num <= 100
}

void imprime_vetor(float *vet) {
    for (int i = 0 ; i < n ; i++)
        printf("%.1f  ", vet[i]);
    printf("\n");
}

/**
 * Calcula a quantidade de valores que estao entre um limite inferior
 * inf e um superior sup em um dado vetor de forma sequencial
 */
long long int qtd_valores_sequencial(float *vet, float inf, float sup) {
    long long int qtd = 0;
    for (long long int i = 0 ; i < n ; i++) {
        if (vet[i] > inf && vet[i] < sup)
            qtd += 1;
    }
    return qtd;
}


int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf(COLOR_BOLD_YELLOW "Execute como: %s <tamanho> <limite inferior>", argv[0]);
        printf(" <limite superior> <numero de threads>\n" COLOR_RESET);
        return 1;
    }
    n = atoll(argv[1]);
    inferior = atof(argv[2]);
    superior = atof(argv[3]);
    n_threads = atoi(argv[4]);
    double inicio, fim;

    GET_TIME(inicio);
    // aloca e preenche o vetor
    vetor = (float *) malloc(n * sizeof(float));
    if (vetor == NULL) {
        printf(COLOR_BOLD_RED "Erro ao alocar o vetor\n" COLOR_RESET);
        exit(1);
    }
    preenche_vetor(vetor);
    // imprime_vetor(vetor);
    GET_TIME(fim);
    double tempo_aloc = fim - inicio;

    GET_TIME(inicio);
    // realiza o algoritmo de forma sequencial
    long long int qtd_sequencial = qtd_valores_sequencial(vetor, inferior, superior);
    GET_TIME(fim);
    double tempo_seq = fim - inicio;


    printf("Tempo de alocacao e preenchimento: %.5lf\n", tempo_aloc);
    printf("Tempo de execucao sequencial: %.5lf\n\n", tempo_seq);
    printf("Quantidade sequencial: %lld\n", qtd_sequencial);

    free(vetor);
    return 0;
}