#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 5

int var = 0; // variavel compartilhada entre as threads
pthread_mutex_t mutex;  // variavel de exclusao mutua
pthread_cond_t cond;    // variavel de sincronizacao

void *t1 (void *arg) {
    while (var < 4) {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("(1) Volte sempre!\n");
    pthread_exit(NULL);
}

void *t2 (void *arg) {
    pthread_mutex_lock(&mutex);
    // espera receber sinal da thread 5
    while (var < 1) {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("(2) Fique a vontade.\n");
    var++;
    pthread_cond_signal(&cond); // envia sinal para a thread 1
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *t3 (void *arg) {
    pthread_mutex_lock(&mutex);
    // espera receber sinal da thread 5
    while (var < 1) {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("(3) Sente-se por favor.\n");
    var++;
    pthread_cond_signal(&cond); // envia sinal para a thread 1
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *t4 (void *arg) {
    pthread_mutex_lock(&mutex);
    // espera receber sinal da thread 5
    while (var < 1) {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("(4) Aceita um copo d'agua?\n");
    var++;
    pthread_cond_signal(&cond); // envia sinal para a thread 1
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *t5 (void *arg) {
    pthread_mutex_lock(&mutex);
    printf("(5) Seja bem-vindo!\n");
    var++;
    pthread_cond_broadcast(&cond); // envia sinal para threads 2, 3 e 4
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
    pthread_cond_init(&cond, NULL);

    pthread_create(tids_sistema + 0, NULL, t1, NULL);
    pthread_create(tids_sistema + 1, NULL, t2, NULL);
    pthread_create(tids_sistema + 2, NULL, t3, NULL);
    pthread_create(tids_sistema + 3, NULL, t4, NULL);
    pthread_create(tids_sistema + 4, NULL, t5, NULL);

    free(tids_sistema);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    
    pthread_exit(NULL);
    return 0;
}
