#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define COLOR_PURPLE "\033[0;35m"
#define COLOR_BLUE "\e[1;34m"
#define COLOR_RESET "\x1b[0m"
#define N 6

int produtores, consumidores, *buffer;
int in = 0, out = 0, qtd = 0;
sem_t vazio, cheio, mutex_prod, mutex_cons;

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
    sem_wait(&vazio);

    sem_wait(&mutex_prod);
    int posts = 0;
    for (int i = qtd ; i < N ; i++) {
        qtd++, posts++;
        buffer[in] = elemento;
        in = (in + 1) % N;
    }
    for (int i = 0 ; i < posts ; i++) {
        sem_post(&cheio);
        sem_wait(&vazio);
    }
    imprime(buffer, "Prod", id, 1);
    sem_post(&mutex_prod);
}

int consome(int id) {
    sem_wait(&cheio);

    sem_wait(&mutex_cons);
    qtd--;
    int elemento = buffer[out];
    buffer[out] = 0;
    out = (out + 1) % N;
    imprime(buffer, "Cons", id, 2);
    sem_post(&mutex_cons);

    sem_post(&vazio);
    return elemento;
}

void *produtor(void *arg) {
    srand(time(NULL));
    int id = * (int *) arg;
    while (1) {
        // 1 <= elemento <= 9
        int elemento = ((double) rand() / ((double) RAND_MAX + 1)) * 9 + 1;
        produz(elemento, id);
    }
    pthread_exit(NULL);
}

void *consumidor(void *arg) {
    int elemento, id = * (int *) arg;
    while (1)
        elemento = consome(id);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Execute como: %s <quantidade de produtores> <quantidade de consumidores>\n", argv[0]);
        return 0;
    }
    produtores = atoi(argv[1]);
    consumidores = atoi(argv[2]);

    sem_init(&vazio, 0, N);
    sem_init(&cheio, 0, 0);
    sem_init(&mutex_prod, 0, 1); // semaforo binario
    sem_init(&mutex_cons, 0, 1); // semaforo binario

    buffer = (int *) malloc(N * sizeof(int));
    for (int i = 0 ; i < N ; i++)
        buffer[i] = 0;
    
    printf("Buffer inicial: [  ");
    for (int i = 0 ; i < N ; i++)
        printf("%d  ", buffer[i]);
    printf("]\n");

    pthread_t tids_prod[produtores];
    pthread_t tids_cons[consumidores];
    for (int i = 0 ; i < produtores ; i++) {
        if (pthread_create(&tids_prod[i], NULL, produtor, (void *) &i)) {
            printf("Erro na funcao pthread_create()\n");
            exit(1);
        }
    }

    for (int i = 0 ; i < consumidores ; i++) {
        if (pthread_create(&tids_cons[i], NULL, consumidor, (void *) &i)) {
            printf("Erro na funcao pthread_create()\n");
            exit(1);
        }
    }

    for (int i = 0 ; i < produtores ; i++)
        if (pthread_join(tids_prod[i], NULL)) {
            printf("Erro na funcao pthread_join()\n");
            exit(1);
        }

    for (int i = 0 ; i < consumidores ; i++)
        if (pthread_join(tids_cons[i], NULL)) {
            printf("Erro na funcao pthread_join()\n");
            exit(1);
        }

    for (int i = 0 ; i < N; i++) {
        buffer[i] = ((double) rand() / ((double) RAND_MAX + 1)) * 9 + 1;
    }
    // imprime(buffer);

    sem_destroy(&vazio);
    sem_destroy(&cheio);
    sem_destroy(&mutex_prod);
    sem_destroy(&mutex_cons);
    free(buffer);

    return 0;
}