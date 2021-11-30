#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "timer.h"
#include <unistd.h>
#include <pthread.h>

#define COLOR_BOLD_BLUE "\e[1;34m"
#define COLOR_BOLD_GREEN "\e[1;32m"
#define COLOR_BOLD_RED "\e[1;31m"
#define COLOR_BOLD_YELLOW "\e[1;33m"
#define COLOR_RESET "\x1b[0m"

typedef struct {
    int id; // identificador da linha que a thread vai processar
    int dim;
} Args;

int dimensao, n_threads;
int **matriz_1, **matriz_2, **saida, **saida_sequencial; 

// preenche matriz com numeros aleatorios
void preenche_matriz(int **mat) {
    srand(time(NULL));
    for (int i = 0 ; i < dimensao ; i++) {
        for (int j = 0 ; j < dimensao ; j++)
            mat[i][j] = rand() % 100; // 0 <= num <= 99
    }
}

void imprime_matriz(int **mat) {
    for (int i = 0 ; i < dimensao ; i++) {
        printf("["); 
        for (int j = 0 ; j < dimensao ; j++)
            printf("%7d", mat[i][j]);
        printf(" ]\n"); 
    }
}

void multiplica_matrizes_sequencialmente(int **mat_1, int **mat_2) {
    for (int i = 0 ; i < dimensao ; i++) {
        for (int j = 0 ; j < dimensao ; j++) {
            for (int k = 0 ; k < dimensao ; k++)
                saida_sequencial[i][j] += mat_1[i][k] * mat_2[k][j];
        }
    }
}

void *multiplica_matrizes(void *arg) {
    Args *e = (Args *) arg;
    for (int i = e->id ; i < e->dim ; i += n_threads) {
        for (int j = 0 ; j < e->dim ; j++) {
            for (int k = 0 ; k < e->dim ; k++)
                saida[i][j] += matriz_1[i][k] * matriz_2[k][j];
        }
    }
    pthread_exit(NULL);
}

int teste(int **mat_1, int **mat_2) {
    for (int i = 0 ; i < dimensao ; i++) {
        for (int j = 0 ; j < dimensao ; j++) {
            if (mat_1[i][j] != mat_2[i][j]) {
                printf("Erro na posicao [%d][%d]\n", i, j);
                return 0; // as matrizes nao sao iguais
            }
        }
    }
    return 1; // as matrizes sao iguais
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf(COLOR_BOLD_YELLOW "Execute como: %s <dimensao> <numero de threads>\n" COLOR_RESET, argv[0]);
        return 1;
    }
    dimensao = atoi(argv[1]);
    n_threads = atoi(argv[2]);
    if (n_threads > dimensao)
        n_threads = dimensao;
    
    double inicio, fim;

    GET_TIME(inicio);
    // aloca e preenche a primeira matriz
    matriz_1 = (int **) malloc(dimensao * sizeof(int*));
    if (matriz_1 == NULL) {
        printf(COLOR_BOLD_RED "Erro ao alocar primeira matriz\n" COLOR_RESET);
        exit(1);
    }
    for (int i = 0 ; i < dimensao ; i++) {
        matriz_1[i] = (int *) malloc(dimensao * sizeof(int*)); 
        if (matriz_1[i] == NULL) {
            printf(COLOR_BOLD_RED "Erro ao alocar primeira matriz\n" COLOR_RESET);
            exit(1);
        }
    }
    preenche_matriz(matriz_1);

    sleep(1); // pausa a execucao para gerar matrizes diferentes

    // aloca e preenche a segunda matriz
    matriz_2 = (int **) malloc(dimensao * sizeof(int*));
    if (matriz_2 == NULL) {
        printf(COLOR_BOLD_RED "Erro ao alocar segunda matriz\n" COLOR_RESET);
        exit(1);
    }
    for (int i = 0 ; i < dimensao ; i++) {
        matriz_2[i] = (int *) malloc(dimensao * sizeof(int*));
        if (matriz_2[i] == NULL) {
            printf(COLOR_BOLD_RED "Erro ao alocar segunda matriz\n" COLOR_RESET);
            exit(1);
        }
    }
    preenche_matriz(matriz_2);

    // aloca e preenche a matriz de saida
    saida = (int **) malloc(dimensao * sizeof(int*));
    if (saida == NULL) {
        printf(COLOR_BOLD_RED "Erro ao alocar matriz de saida\n" COLOR_RESET);
        exit(1);
    }
    for (int i = 0 ; i < dimensao ; i++) {
        saida[i] = (int *) malloc(dimensao * sizeof(int *));
        if (saida[i] == NULL) {
            printf(COLOR_BOLD_RED "Erro ao alocar matriz de saida\n" COLOR_RESET);
            exit(1);
        }
    }
    for (int i = 0 ; i < dimensao ; i++) {
        for (int j = 0 ; j < dimensao ; j++)
            saida[i][j] = 0; // zera a matriz de saida
    }
    GET_TIME(fim);
    double tempo_alocacao = fim - inicio;

    GET_TIME(inicio);
    pthread_t tids_sistema[n_threads];
    Args *args; // recebe os argumentos para cada thread

    // cria novas threads
    for (int i = 0 ; i < n_threads ; i++) {
        args = (Args *) malloc(sizeof(Args));
        if (args == NULL) {
            printf(COLOR_BOLD_RED "Erro ao aolocar argumentos\n" COLOR_RESET);
            exit(1);
        }
        args->id = i;
        args->dim = dimensao;

        if (pthread_create(&tids_sistema[i], NULL, multiplica_matrizes, (void *) args)) {
            printf(COLOR_BOLD_RED "Erro ao criar thread\n" COLOR_RESET);
            exit(1);
        }
    }

    // faz com que o fluxo principal espere o fim das threads criadas
	for (int i = 0 ; i < n_threads ; i++) {
		if (pthread_join(tids_sistema[i], NULL)) {
			printf(COLOR_BOLD_RED "Erro na funcao pthread_join()\n" COLOR_RESET);
			exit(1);
		}
	}
    GET_TIME(fim);
    double tempo_concorrente = fim - inicio;

    saida_sequencial = (int **) malloc(dimensao * sizeof(int*));
    if (saida_sequencial == NULL) {
        printf(COLOR_BOLD_RED "Erro ao alocar matriz de saida sequencial\n" COLOR_RESET);
        exit(1);
    }
    for (int i = 0 ; i < dimensao ; i++) {
        saida_sequencial[i] = (int *) malloc(dimensao * sizeof(int *));
        if (saida_sequencial[i] == NULL) {
            printf(COLOR_BOLD_RED "Erro ao alocar matriz de saida sequencial\n" COLOR_RESET);
            exit(1);
        }
    }
    for (int i = 0 ; i < dimensao ; i++) {
        for (int j = 0 ; j < dimensao ; j++)
            saida_sequencial[i][j] = 0;
    }

    GET_TIME(inicio);
    multiplica_matrizes_sequencialmente(matriz_1, matriz_2);
    GET_TIME(fim);
    double tempo_sequencial = fim - inicio;

    printf("Tempo de alocacao de matrizes: %.5lf segundos\n", tempo_alocacao - 1);
    printf("Tempo de multiplicacao sequencial: %.5lf segundos\n", tempo_sequencial);
    printf("Tempo de multiplicacao concorrente: %.5lf segundos\n", tempo_concorrente);

    int t = teste(saida, saida_sequencial);
    if (t) printf(COLOR_BOLD_BLUE "\nSucesso\n\n" COLOR_RESET);
    else printf(COLOR_BOLD_RED "\nFalha\n\n" COLOR_RESET);

    free(matriz_1); free(matriz_2);
    free(saida); free(saida_sequencial);
    return 0;
}