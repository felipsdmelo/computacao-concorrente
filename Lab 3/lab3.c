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

long long int n, ret; // dimensao do vetor e valor de retorno das threads
float inferior, superior, *vetor;
int n_threads;

void preenche_vetor(float *vet) {
    srand(time(NULL));
    for (int i = 0 ; i < n ; i++)
        vet[i] = (float) rand() / (float) RAND_MAX * 1000; // 0 <= num <= 1000
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

/**
 * Calcula a quantidade de valores que estao entre um limite inferior
 * e um superior em um dado vetor de forma concorrente
 */
void *qtd_valores_concorrente(void *arg) {
    int id = (long long int) arg; // identificador da thread
    long long int qtd_local = 0;
    long long int tam_bloco = n / n_threads;
    long long int inicio = id * tam_bloco;
    long long int fim;

    if (id == n_threads - 1) fim = n;
    else fim = inicio + tam_bloco;

    for (long long int i = inicio ; i < fim ; i++) {
        if (vetor[i] > inferior && vetor[i] < superior)
            qtd_local += 1;
    }

    pthread_exit((void *) qtd_local);
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
    GET_TIME(fim);
    double tempo_aloc = fim - inicio;


    GET_TIME(inicio);
    // realiza o algoritmo de forma sequencial
    long long int qtd_sequencial = qtd_valores_sequencial(vetor, inferior, superior);
    GET_TIME(fim);
    double tempo_seq = fim - inicio;


    GET_TIME(inicio);
    pthread_t *tids_sistema;
    tids_sistema = (pthread_t *) malloc(n_threads * sizeof(pthread_t));
    if (tids_sistema == NULL) {
        printf(COLOR_BOLD_RED "Erro ao alocar os identificadores das threads do sistema\n" COLOR_RESET);
        exit(1);
    }
    // cria novas threads para a execucao concorrente
    for (long long int i = 0 ; i < n_threads ; i++) {
        if (pthread_create(tids_sistema + i, NULL, qtd_valores_concorrente, (void *) i)) {
            printf(COLOR_BOLD_RED "Erro ao criar threads\n" COLOR_RESET);
            exit(1);
        }
    }

    long long int qtd_concorrente = 0;
    // faz com que o fluxo principal espere o fim das threads criadas
	for (long long int i = 0 ; i < n_threads ; i++) {
		if (pthread_join(*(tids_sistema + i), (void **) &ret)) {
			printf(COLOR_BOLD_RED "Erro na funcao pthread_join()\n" COLOR_RESET);
			exit(1);
		}
        qtd_concorrente += ret;
	}
    GET_TIME(fim);
    double tempo_conc = fim - inicio;

    printf("Tempo de alocacao e preenchimento: %.5lf\n\n", tempo_aloc);
    printf("Tempo de execucao sequencial: %.5lf\n", tempo_seq);
    printf("Tempo de execucao concorrente: %.5lf\n\n", tempo_conc);
    printf("Quantidade sequencial: %lld\n", qtd_sequencial);
    printf("Quantidade concorrente: %lld\n", qtd_concorrente);

    if (qtd_sequencial != qtd_concorrente) printf(COLOR_BOLD_RED "\nFalha\n\n" COLOR_RESET);
    else printf(COLOR_BOLD_BLUE "\nSucesso\n\n" COLOR_RESET);

    free(vetor); free(tids_sistema);
    return 0;
}