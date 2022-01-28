#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "timer.h"

#define COLOR_BLUE "\e[1;34m"
#define COLOR_RED "\e[1;31m"
#define COLOR_YELLOW "\e[1;33m"
#define COLOR_RESET "\x1b[0m"

// estrutura de dados para passar mais de um argumento para as threads
typedef struct {
    int id;
    long long int inicio;
    long long int fim;
} Args;

int *vetor, *vetorSeq, *auxiliar;
long long int N; // dimensao do vetor
int N_THREADS;

void preenche_vetor(int *vet) {
    srand(time(NULL));
    for (int i = 0 ; i < N ; i++)
        vet[i] = rand() % 10000; // 0 < x < 9999
    // int val = N - 1;
    // for (int i = 0 ; i < N ; i++) {
    //     vet[i] = val;
    //     val--;
    // }
}

/**
 * @brief: Imprime um vetor
 * 
 * @param vet: vetor que sera impresso
 * @param msg: mensagem
 */
void imprime(int *vet, char msg[]) {
    printf("%s: [  ", msg);
    for (int i = 0 ; i < N ; i++) {
        printf("%d  ", vet[i]);
    }
    printf("]\n");
}

// void merge(int *vet, long long int inicio, long long int meio, long long int fim) {
//     int *esq, *dir;
//     esq = (int *) malloc((meio - inicio) * sizeof(int));
//     dir = (int *) malloc((fim - meio) * sizeof(int));
//     int index_esq = 0, index_dir = 0;
//     for (int i = 0 ; i < fim ; i++) {
//         if (index_esq >= meio - inicio) {
//             vet[i] = dir[index_dir];
//             index_dir += 1;
//         }
//         else if (index_dir >= fim - meio) {
//             vet[i] = esq[index_esq];
//             index_esq += 1;
//         }
//         else if (esq[index_esq] <= dir[index_dir]) {
//             vet[i] = esq[index_esq];
//             index_esq += 1;
//         }
//         else {
//             vet[i] = dir[index_dir];
//             index_dir += 1;
//         }
//     }
//     free(esq); free(dir);
// }

/**
 * @brief: Realiza o merge entre os subvetores que cada thread ordenou
 * 
 * @param vet: vetor que sera fundido
 * @param vet_aux: vetor auxiliar com a mesma dimensao de vet
 * @param inicio: posicao de inicio do vetor
 * @param meio: posicao central do vetor
 * @param fim: tamanho do vetor
 */
void merge(int *vet, int *vet_aux, int inicio, int meio, int fim) {
    long long int index_esq = inicio;
    long long int index_dir = meio;
    long long int fim_esq = meio;
    long long int fim_dir = fim;

    long long int index = inicio;

    // compara os menores valores de cada subvetor e copia para
    // o vetor auxiliar o elemento de menor valor
    while (index_esq < fim_esq && index_dir < fim_dir) {
        if (vet[index_esq] < vet[index_dir]) {
            vet_aux[index++] = vet[index_esq++];
        }
        else {
            vet_aux[index++] = vet[index_dir++];
        }
    }
    
    // copia o resto do vetor que falta, esquerda ou direita
    if (index_esq >= fim_esq) {
        for (int i = index_dir ; i < fim_dir ; i++)
            vet_aux[index++] = vet[i];
    }
    else if (index_dir >= fim_dir) {
        for (int i = index_esq ; i < fim_esq ; i++)
            vet_aux[index++] = vet[i];
    }
    
    for (long long int i = 0 ; i < fim - inicio ; i++) {
        vet[i + inicio] = vet_aux[inicio + i];
    }
}

/**
 * @brief: Realiza a ordenacao de um vetor utilizando o algoritmo QuickSort
 * 
 * @param vet: vetor que sera ordenado 
 * @param inicio: posicao de inicio do vetor
 * @param fim: tamanho do vetor
 */
void quicksort(int *vet, long long int inicio, long long int fim) {
    long long int i = inicio;
    long long int j = fim - 1;
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

/**
 * @brief: Verifica se as ordenacoes foram realizadas de maneira correta
 * 
 * @param vet: vetor que sera testado
 * @param msg: mensagem
 */
void verifica(int *vet, char msg[]) {
    for (int i = 0 ; i < N - 1 ; i++) {
        if (!(vet[i] <= vet[i + 1])) {
            printf(COLOR_RED "%s: Erro na ordenacao do vetor\n" COLOR_RESET, msg);
            return;
        }
    }
    printf(COLOR_BLUE "%s: Ordenacao feita corretamente\n" COLOR_RESET, msg);
}

void *tarefa(void *_arg) {
    Args *x = (Args *) _arg;
    if (x->id == N_THREADS - 1)
        x->fim = N;
    quicksort(vetor, x->inicio, x->fim);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf(COLOR_YELLOW "Execute como: %s <tamanho do vetor> <numero de threads>\n" COLOR_RESET, argv[0]);
        return 0;
    }
    N = atoll(argv[1]);
    N_THREADS = atoi(argv[2]);

    double inicio, fim;

    GET_TIME(inicio); // tempo de alocacao
    vetor = (int *) malloc(N * sizeof(int));
    vetorSeq = (int *) malloc(N * sizeof(int));
    auxiliar = (int *) malloc(N * sizeof(int));
    GET_TIME(fim) // tempo de alocacao
    double t_alocacao = fim - inicio;

    GET_TIME(inicio);
    preenche_vetor(vetor);
    for (int i = 0 ; i < N ; i++) {
        vetorSeq[i] = vetor[i];
    }
    GET_TIME(fim);
    double t_preenchimento = fim - inicio;

    // imprime(vetor, "Original Conc");
    // imprime(vetorSeq, "Original Seq");

    GET_TIME(inicio); // tempo de execucao do algoritmo concorrente
    // cria as threads da aplicacao
    Args *args;
    pthread_t tids_sistema[N_THREADS];
    for (int i = 0 ; i < N_THREADS ; i++) {
        args = (Args *) malloc(sizeof(Args));
        args->id = i;
        args->inicio = (N / N_THREADS) * i;
        args->fim = (N / N_THREADS) * (i + 1);
        if (pthread_create(&tids_sistema[i], NULL, tarefa, (void *) args)) {
            printf(COLOR_RED "Erro na funcao pthread_create()\n" COLOR_RESET);
            exit(1);
        }
    }

    // aguarda o termino das threads
    for (int i = 0 ; i < N_THREADS ; i++) {
        if(pthread_join(tids_sistema[i], NULL)) {
            printf(COLOR_RED "Erro na funcao pthread_join()\n" COLOR_RESET);
            exit(1);
        }
    }
    GET_TIME(fim); // tempo de execucao do algoritmo concorrente
    double t_concorrente = fim - inicio;


    GET_TIME(inicio); // tempo de execucao do merge
    // funde os subvetores gerados pelas threads
    long long int tam_bloco = N / N_THREADS;
    int i;
    // funde os subvetores em duplas
    for (i = 0 ; i < N_THREADS - 2 ; i++)
        merge(vetor, auxiliar, 0, (i + 1) * tam_bloco, (i + 1) * tam_bloco + tam_bloco);
    merge(vetor, auxiliar, 0, (i + 1) * tam_bloco, N);
    free(auxiliar);
    GET_TIME(fim); // tempo de execucao do merge
    double t_merge = fim - inicio;

    // imprime(vetor, "Concorrente ordenado");

    GET_TIME(inicio); // tempo de execucao do algoritmo sequencial
    quicksort(vetorSeq, 0, N);
    GET_TIME(fim); // tempo de execucao do algoritmo sequencial
    double t_sequencial = fim - inicio;
    // imprime(vetorSeq, "Sequencial ordenado");

    printf("\n");
    GET_TIME(inicio); // tempo de verificacao dos vetores
    verifica(vetor, "Concorrente");
    verifica(vetorSeq, "Sequencial");
    GET_TIME(fim); // tempo de verificacao dos vetores
    double t_verificacao = fim - inicio;

    printf(COLOR_YELLOW "\nTempo de alocacao ----------------------------------- %.5lf seg\n", t_alocacao);
    printf("Tempo de preenchimento dos vetores ------------------ %.5lf seg\n", t_preenchimento);
    printf("Tempo do merge -------------------------------------- %.5lf seg\n", t_merge);
    printf("Tempo de execucao do algoritmo concorrente: --------- %.5lf seg\n", t_concorrente + t_merge);
    printf("Tempo de execucao do algoritmo sequencial: ---------- %.5lf seg\n", t_sequencial);
    printf("Tempo do teste de corretude ------------------------- %.5lf seg\n", t_verificacao);
    double total = t_alocacao + t_preenchimento + t_sequencial + t_concorrente + t_merge + t_verificacao;
    printf("\nTaxa sequencial/concorrente: %.5lf\n", t_sequencial / (t_concorrente + t_merge));
    printf("\nTempo total: %.5lf seg\n" COLOR_RESET, total);

    free(args); free(vetor); free(vetorSeq);

    return 0;
}
