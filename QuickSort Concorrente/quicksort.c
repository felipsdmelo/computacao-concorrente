#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define COLOR_BLUE "\e[1;34m"
#define COLOR_RED "\e[1;31m"
#define COLOR_YELLOW "\e[1;33m"
#define COLOR_RESET "\x1b[0m"

typedef struct {
    int inicio;
    int fim;
} Args;
int *vetor;
int N;

void preenche_vetor(int *vet) {
    srand(time(NULL));
    for (int i = 0 ; i < N ; i++)
        vet[i] = rand() % 10000;
    // int val = N - 1;
    // for (int i = 0 ; i < N ; i++) {
    //     vet[i] = val;
    //     val--;
    // }
}

void imprime(int *vet) {
    printf("[  ");
    for (int i = 0 ; i < N ; i++) {
        printf("%d  ", vet[i]);
    }
    printf("]\n");
}

void quicksort(int *vet, int inicio, int fim) {
    int i = inicio;
    int j = fim - 1;
    int pivo = vet[(inicio + fim) / 2];
    while (i <= j) {
        while (vet[i] < pivo && i < fim)
            i++;
        while (vet[j] > pivo && j > inicio)
            j--;
        if (i <= j) {
            int aux = vet[i];
            vet[i] = vet[j];
            vet[j] = aux;
            i++; j--;
        }
    }

    if (j > inicio)
        quicksort(vet, inicio, j + 1);
    if (i < fim)
        quicksort(vet, i, fim);
}


void verifica(int *vet) {
    for (int i = 0 ; i < N - 1 ; i++) {
        if (!(vet[i] <= vet[i + 1])) {
            printf(COLOR_RED "Erro na ordenacao do vetor\n" COLOR_RESET);
            return;
        }
    }
    printf(COLOR_BLUE "Ordenacao feita corretamente\n" COLOR_RESET);
}

void *tarefa(void *_arg) {
    Args *a = (Args *) _arg;
    quicksort(vetor, a->inicio, a->fim);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf(COLOR_YELLOW "Execute como: %s <tamanho do vetor> <numero de threads>\n" COLOR_RESET, argv[0]);
        exit(1);
    }
    N = atoi(argv[1]);
    int n_threads = atoi(argv[2]);

    Args *args;
    vetor = (int *) malloc(N * sizeof(int));

    preenche_vetor(vetor);
    imprime(vetor);

    pthread_t tids_sistema[n_threads];
    for (int i = 0 ; i < n_threads ; i++) {
        args = (Args *) malloc(sizeof(Args));
        args->inicio = (N / n_threads) * i;
        args->fim = (N / n_threads) * (i + 1) - 1;
        if (pthread_create(&tids_sistema[i], NULL, tarefa, (void *) args)) {
            printf(COLOR_RED "Erro na funcao pthread_create()\n" COLOR_RESET);
            exit(1);
        }
    }

    for (int i = 0 ; i < n_threads ; i++) {
        if(pthread_join(tids_sistema[i], NULL)) {
            printf(COLOR_RED "Erro na funcao pthread_join()\n" COLOR_RESET);
            exit(1);
        }
    }
    quicksort(vetor, 0, N);
    imprime(vetor);

    printf("\n");
    verifica(vetor);

    return 0;
}