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
    int id; // identificador da linha que a thread vai processar
    int dim;
} Args;

int dimensao, n_threads;
int **matriz_1;
int **matriz_2;
int **saida;

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
    Args *e = (Args *) arg;
    for (int i = e->id ; i < e->dim ; i += n_threads) {
        for (int j = 0 ; j < e->dim ; j++) {
            for (int k = 0 ; k < e->dim ; k++)
                saida[i][j] += matriz_1[i][k] * matriz_2[k][j];
        }
    }
    pthread_exit(NULL);
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
            saida[i][j] = 0;
    }

    pthread_t tids_sistema[n_threads];
    Args *args; // recebe os argumentos para cada thread

    printf("Matriz 1:\n"); imprime_matriz(matriz_1);printf("\n\n");
    printf("Matriz 2:\n"); imprime_matriz(matriz_2);printf("\n\n");

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

    printf("Saida:\n"); imprime_matriz(saida);printf("\n\n");

    return 0;
}