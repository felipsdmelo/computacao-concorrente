#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define COLOR_BOLD_BLUE "\e[1;34m"
#define COLOR_BOLD_GREEN "\e[1;32m"
#define COLOR_BOLD_RED "\e[1;31m"
#define COLOR_BOLD_YELLOW "\e[1;33m"
#define COLOR_RESET "\x1b[0m"

typedef struct {
    int id; // identificador local das trheads
    int dim;
} Args;

int dimensao, n_threads;
int **matriz_1;
int **matriz_2;

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
            printf("%3d", mat[i][j]);
        printf(" ]\n"); 
    }
}

void *multiplica_matrizes(void *arg) {
    Args *args = (Args *) arg;
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
    
    // aloca e preenche a primeira matriz
    matriz_1 = (int **) malloc(dimensao * sizeof(int*));
    for (int i = 0 ; i < dimensao ; i++)
        matriz_1[i] = (int *) malloc(dimensao * sizeof(int*));
    preenche_matriz(matriz_1);

    sleep(1); // pausa a execucao para gerar matrizes diferentes

    // aloca e preenche a segunda matriz
    matriz_2 = (int **) malloc(dimensao * sizeof(int*));
    for (int i = 0 ; i < dimensao ; i++)
        matriz_2[i] = (int *) malloc(dimensao * sizeof(int*));
    preenche_matriz(matriz_2);

    // pthread_t tids_sistema[NTHREADS];
    // int id_thread[NTHREADS]; // identificador local de cada thread
    return 0;
}