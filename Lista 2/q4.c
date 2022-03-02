#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define COLOR_BLUE "\e[1;34m"
#define COLOR_YELLOW "\e[0;33m"
#define COLOR_RESET "\x1b[0m"

int *fila, N, qtd = 0, n_produtoras, n_impressoras;
int in = 0, out = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_prod, cond_cons;

int gera_numero() {
    return rand() % 100; // 0 <= num <= 99
}

void imprime(int *v) {
    printf("Fila atual: [  ");
    for (int i = 0 ; i < N ; i++)
        printf("%d  ", v[i]);
    printf("]\n");
}

void insere(int num, int id) {
    pthread_mutex_lock(&mutex);
    while (qtd == N) { // fila cheia
        printf("Produtora(%d): produtora bloqueada\n", id);
        pthread_cond_wait(&cond_prod, &mutex);
    }
    qtd++;
    fila[in] = num; // insere elemento na fila
    printf(COLOR_YELLOW "Produtora(%d): inseriu elemento %d\n" COLOR_RESET, id, num);
    in = (in + 1) % N;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond_cons);
}

int retira(int id) {
    pthread_mutex_lock(&mutex);
    while (!qtd) { // fila vazia
        printf("Impressora(%d): impressora bloqueada\n", id);
        pthread_cond_wait(&cond_cons, &mutex);
    }
    qtd--;
    int e = fila[out];
    fila[out] = -1; // libera a posicao da fila
    printf("Impressora(%d): retirou elemento %d\n", id, e);
    out = (out + 1) % N;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond_prod);
    return e;
}

void *produtora(void *arg) {
    int id = * (int *) arg;
    while (1) {
        usleep(150000);
        int e = gera_numero();
        printf("Produtora(%d): produziu elemento %d\n", id, e);
        insere(e, id);
        pthread_mutex_lock(&mutex);
        printf("Produtora(%d) - ", id);
        imprime(fila);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void *impressora(void *arg) {
    int id = * (int *) arg;
    while (1) {
        usleep(150000);
        int e = retira(id);
        pthread_mutex_lock(&mutex);
        printf(COLOR_BLUE "Impressora(%d): imprimiu elemento %d\n" COLOR_RESET, id, e);
        printf("Impressora(%d) - ", id);
        imprime(fila);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc < 4) {
        printf("Execute como: %s <numero de produtoras>", argv[0]);
        printf(" <numero de impressoras> <tamanho da fila>\n");
        return 1;
    }
    n_produtoras = atoi(argv[1]);
    n_impressoras = atoi(argv[2]);
    N = atoi(argv[3]);

    // alocacoes
    fila = (int *) malloc(N * sizeof(int));
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_prod, NULL);
    pthread_cond_init(&cond_cons, NULL);

    // inicializacao da fila
    for (int i = 0 ; i < N ; i++)
        fila[i] = -1; // posicao vazia

    // identificadores das threads no sistema
    pthread_t tids_prod[n_produtoras];
    pthread_t tids_imp[n_impressoras];

    // cria threads produtoras
    for (int i = 0 ; i < n_produtoras ; i++) {
        if (pthread_create(&tids_prod[i], NULL, produtora, (void *) &i)) {
            printf("Erro na funcao pthread_create()\n");
            exit(1);
        }
    }

    // cria threads impressoras
    for (int i = 0 ; i < n_impressoras ; i++) {
        if (pthread_create(&tids_imp[i], NULL, impressora, (void *) &i)) {
            printf("Erro na funcao pthread_create()\n");
            exit(1);
        }
    }

    // aguarda o fim das threads produtoras
    for (int i = 0 ; i < n_produtoras ; i++) {
        if (pthread_join(tids_prod[i], NULL)) {
            printf("Erro na funcao pthread_join()\n");
            exit(1);
        }
    }

    // aguarda o fim das threads impressoras
    for (int i = 0 ; i < n_impressoras ; i++) {
        if (pthread_join(tids_imp[i], NULL)) {
            printf("Erro na funcao pthread_join()\n");
            exit(1);
        }
    }

    // liberacao de memoria
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_prod);
    pthread_cond_destroy(&cond_cons);
    free(fila);

    return 0;
}
