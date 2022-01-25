#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2
#define N 1050 // limite superior que delimita ponto de parada para impressao de multiplos
#define VALOR 100 // valor cujos multiplos queremos imprimir


pthread_mutex_t mutex;
pthread_cond_t cond1, cond2;
int contador = 0, imprime = 0, fim = 0;

void *Thread1(void *arg) {
    for (int i = 0 ; i < N ; i++) {
        contador++;
        if(!(contador % VALOR)) {
            pthread_mutex_lock(&mutex);
            imprime = 1;
            pthread_cond_signal(&cond2);
            pthread_cond_wait(&cond1, &mutex);
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_mutex_lock(&mutex);
    fim = 1;
    pthread_cond_signal(&cond2);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *Thread2(void *arg) {
    pthread_mutex_lock(&mutex);
    while (!fim) {
        if (imprime) {
            printf("Contador = %d\n", contador);
            imprime = 0;
            pthread_cond_signal(&cond1);
        } 
        else
            pthread_cond_wait(&cond2, &mutex);
    }
    pthread_mutex_unlock(&mutex);
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
