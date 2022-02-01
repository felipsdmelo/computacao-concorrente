#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 5

int var = 0; // variavel compartilhada entre as threads
sem_t semaforo_1, semaforo_2;

void *t1 (void *arg) {
    while (var < 3)
        sem_wait(&semaforo_2);
    printf("(1) Volte sempre!\n");
    pthread_exit(NULL);
}

void *t2 (void *arg) {
    // espera receber sinal da thread 5
    sem_wait(&semaforo_1);
    printf("(2) Fique a vontade.\n");
    var++;
    sem_post(&semaforo_2); // envia sinal para a thread 1
    pthread_exit(NULL);
}

void *t3 (void *arg) {
    // espera receber sinal da thread 5
    sem_wait(&semaforo_1);
    printf("(3) Sente-se por favor.\n");
    var++;
    sem_post(&semaforo_2); // envia sinal para a thread 1
    pthread_exit(NULL);
}

void *t4 (void *arg) {
    // espera receber sinal da thread 5
    sem_wait(&semaforo_1);
    printf("(4) Aceita um copo d'agua?\n");
    var++;
    sem_post(&semaforo_2); // envia sinal para a thread 1
    pthread_exit(NULL);
}

void *t5 (void *arg) {
    printf("(5) Seja bem-vindo!\n");
    sem_post(&semaforo_1); // envia sinal para threads 2, 3 e 4
    pthread_exit(NULL);
}

int main() {
    pthread_t *tids_sistema;
    tids_sistema = (pthread_t *) malloc(NTHREADS * sizeof(pthread_t));
    if (tids_sistema == NULL) {
        printf("Erro ao alocar os identificadores das threads do sistema\n");
        exit(1);
    }

    sem_init(&semaforo_1, 0, 0);
    sem_init(&semaforo_2, 0, 0);

    pthread_create(tids_sistema + 1, NULL, t2, NULL);
    pthread_create(tids_sistema + 0, NULL, t1, NULL);
    pthread_create(tids_sistema + 2, NULL, t3, NULL);
    pthread_create(tids_sistema + 4, NULL, t5, NULL);
    pthread_create(tids_sistema + 3, NULL, t4, NULL);

    free(tids_sistema);

    return 0;
}