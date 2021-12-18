#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 5

int main() {
    pthread_t *tids_sistema;
    tids_sistema = (pthread_t *) malloc(NTHREADS * sizeof(pthread_t));
    if (tids_sistema == NULL) {
        printf("Erro ao alocar os identificadores das threads do sistema\n");
        exit(1);
    }

    free(tids_sistema);
    return 0;
}