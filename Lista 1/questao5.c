#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2
#define N 1050 // limite superior que delimita ponto de parada para impressao de multiplos
#define VALOR 100 // valor cujos multiplos queremos imprimir

pthread_mutex_t mutex;
pthread_cond_t cond1, cond2;
int contador = 0;

void *Thread1(void *arg) {
    for (int i = 0 ; i < N ; i++) {
        pthread_mutex_lock(&mutex);
        contador++;
        if (contador % VALOR == 0) { // verifica se contador eh multiplo de 100
            printf("---> ");
            pthread_cond_signal(&cond2); // envia sinal pra thread 2
            pthread_cond_wait(&cond1, &mutex); // espera a thread 2 enviar sinal de volta
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void *Thread2(void *arg) {
    while (N - contador >= VALOR) { // se (N - contador) < VALOR, entao nao ha mais multiplos para imprimir
        pthread_cond_wait(&cond2, &mutex); // espera a thread 1 enviar o sinal
        printf("Contador = %d\n", contador);
        pthread_cond_signal(&cond1); // envia sinal para a thread 1 apos imprimir o valor de contador
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t *tids_sistema;
    tids_sistema = (pthread_t *) malloc(NTHREADS * sizeof(pthread_t));
    if (tids_sistema == NULL) {
        printf("Erro ao alocar os identificadores das threads do sistema\n");
        exit(1);
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond1, NULL);
    pthread_cond_init(&cond2, NULL);

    // cria as threads
    pthread_create(tids_sistema + 0, NULL, Thread1, NULL);
    pthread_create(tids_sistema + 1, NULL, Thread2, NULL);

    // aguarda as threads terminarem
    for (int i = 0 ; i < NTHREADS ; i++)
        pthread_join(*(tids_sistema + i), NULL);

    free(tids_sistema);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond1);
    pthread_cond_destroy(&cond2);

    return 0;
}