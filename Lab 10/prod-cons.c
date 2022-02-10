#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define COLOR_PURPLE "\033[0;35m"
#define COLOR_BLUE "\e[1;34m"
#define COLOR_RESET "\x1b[0m"
#define N 6

int produtores, consumidores, *buffer;
int in = 0, out = 0, qtd = 0;
sem_t prod, cons, mutex;

// COR 1: produtor     COR 2: consumidor
void imprime(int *buf, char msg[], int id, int cor) {
    if (cor == 1) {
        printf(COLOR_BLUE "%s %d: [  ", msg, id);
        for (int i = 0 ; i < N ; i++)
            printf("%d  ", buf[i]);
        printf("]\n" COLOR_RESET);
    }
    else if (cor == 2) {
        printf(COLOR_PURPLE "%s %d: [  ", msg, id);
        for (int i = 0 ; i < N ; i++)
            printf("%d  ", buf[i]);
        printf("]\n" COLOR_RESET);
    }
}

void produz(int elemento, int id) {
    sem_wait(&prod);

    sem_wait(&mutex);
    for (int i = qtd ; i < N ; i++) {
        qtd++;
        buffer[in] = elemento;
        in = (in + 1) % N;
    }

    out = 0;
    imprime(buffer, "Prod", id, 1);
    sem_post(&mutex);

    for (int i = 0 ; i < consumidores ; i++)
        sem_post(&cons);
}

int consome(int id) {
    sem_wait(&cons);

    sem_wait(&mutex);
    qtd--;
    int elemento = buffer[out];
    buffer[out] = 0;
    out = (out + 1) % N;
    imprime(buffer, "Cons", id, 2);

    if (qtd == 0) {
        sem_post(&prod);
    }
    sem_post(&mutex);

    sem_post(&cons);
    return elemento;
}

void *produtor(void *arg) {
    srand(time(NULL));
    int id = * (int *) arg;
    while (1) {
        sleep(1); // gera numeros diferentes
        // 1 <= elemento <= 9
        int elemento = ((double) rand() / ((double) RAND_MAX + 1)) * 9 + 1;
        produz(elemento, id);
    }
    pthread_exit(NULL);
}

void *consumidor(void *arg) {
    int elemento, id = * (int *) arg;
    while (1) {
        elemento = consome(id);
        // printf("Consumiu %d\n", elemento);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Execute como: %s <quantidade de produtores> <quantidade de consumidores>\n", argv[0]);
        return 0;
    }
    produtores = atoi(argv[1]);
    consumidores = atoi(argv[2]);

    // inicializa variaveis de sincronizacao
    sem_init(&prod, 0, 1); // semaforo binario
    sem_init(&cons, 0, 0);
    sem_init(&mutex, 0, 1); // semaforo binario

    // aloca e preenche buffer
    buffer = (int *) malloc(N * sizeof(int));
    for (int i = 0 ; i < N ; i++)
        buffer[i] = 0;
    
    // imprime buffer inicial
    printf("Buffer inicial: [  ");
    for (int i = 0 ; i < N ; i++)
        printf("%d  ", buffer[i]);
    printf("]\n");

    pthread_t tids_prod[produtores]; // identificador das produtoras no sistema
    pthread_t tids_cons[consumidores]; // identificador das consumidoras no sistema

    // cria threads produtoras
    for (int i = 0 ; i < produtores ; i++) {
        if (pthread_create(&tids_prod[i], NULL, produtor, (void *) &i)) {
            printf("Erro na funcao pthread_create()\n");
            exit(1);
        }
    }

    // cria threads consumidoras
    for (int i = 0 ; i < consumidores ; i++) {
        if (pthread_create(&tids_cons[i], NULL, consumidor, (void *) &i)) {
            printf("Erro na funcao pthread_create()\n");
            exit(1);
        }
    }

    // aguarda fim das threads produtoras
    for (int i = 0 ; i < produtores ; i++)
        if (pthread_join(tids_prod[i], NULL)) {
            printf("Erro na funcao pthread_join()\n");
            exit(1);
        }

    // aguarda fim das threads consumidoras
    for (int i = 0 ; i < consumidores ; i++)
        if (pthread_join(tids_cons[i], NULL)) {
            printf("Erro na funcao pthread_join()\n");
            exit(1);
        }

    // desaloca variaveis
    sem_destroy(&prod);
    sem_destroy(&cons);
    sem_destroy(&mutex);
    free(buffer);

    return 0;
}