#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define COLOR_BOLD_BLUE "\e[1;34m"
#define COLOR_BOLD_GREEN "\e[1;32m"
#define COLOR_BOLD_RED "\e[1;31m"
#define COLOR_BOLD_YELLOW "\e[1;33m"
#define COLOR_RESET "\x1b[0m"

typedef struct {
    int id;
    int dim;
} Args;

void *multiplica(void *arg) {
    Args *args = (Args *) arg;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf(COLOR_BOLD_YELLOW "Execute como: %s <dimensao> <numero de threads>\n" COLOR_RESET, argv[0]);
        return 1;
    }
    int dimensao = atoi(argv[1]);
    int n_threads = atoi(argv[2]);
    // pthread_t tids_sistema[NTHREADS];
    // int id_thread[NTHREADS]; // identificador local de cada thread
    return 0;
}