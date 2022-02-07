#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaforo_5, semaforo_1;

void *t1 (void *arg) {
    sem_wait(&semaforo_1);
    sem_wait(&semaforo_1);
    sem_wait(&semaforo_1);
    printf("(1) Volte sempre!\n");
    pthread_exit(NULL);
}

void *t2 (void *arg) {
    // espera receber sinal da thread 5
    sem_wait(&semaforo_5);
    printf("(2) Fique a vontade.\n");
    sem_post(&semaforo_1);
    pthread_exit(NULL);
}

void *t3 (void *arg) {
    // espera receber sinal da thread 5
    sem_wait(&semaforo_5);
    printf("(3) Sente-se por favor.\n");
    sem_post(&semaforo_1);
    pthread_exit(NULL);
}

void *t4 (void *arg) {
    // espera receber sinal da thread 5
    sem_wait(&semaforo_5);
    printf("(4) Aceita um copo d'agua?\n");
    sem_post(&semaforo_1);
    pthread_exit(NULL);
}

void *t5 (void *arg) {
    printf("(5) Seja bem-vindo!\n");
    // envia sinais para threads 2, 3 e 4
    sem_post(&semaforo_5);
    sem_post(&semaforo_5);
    sem_post(&semaforo_5);
    pthread_exit(NULL);
}

int main() {
    pthread_t *tids_sistema;
    tids_sistema = (pthread_t *) malloc(5 * sizeof(pthread_t));
    if (tids_sistema == NULL) {
        printf("Erro ao alocar os identificadores das threads do sistema\n");
        exit(1);
    }

    sem_init(&semaforo_1, 0, 0);
    sem_init(&semaforo_5, 0, 0);

    pthread_create(tids_sistema + 1, NULL, t2, NULL);
    pthread_create(tids_sistema + 0, NULL, t1, NULL);
    pthread_create(tids_sistema + 2, NULL, t3, NULL);
    pthread_create(tids_sistema + 4, NULL, t5, NULL);
    pthread_create(tids_sistema + 3, NULL, t4, NULL);

    for (int i = 0 ; i < 5 ; i++) {
        if (pthread_join(tids_sistema[i], NULL)) {
            printf("Erro na funcao pthread_join()\n");
            exit(1);
        }
    }
    
    sem_destroy(&semaforo_1);
    sem_destroy(&semaforo_5);
    free(tids_sistema);
    printf("\n");

    return 0;
}
