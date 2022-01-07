#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define COLOR_BOLD_GREEN "\e[1;32m"
#define COLOR_BOLD_YELLOW "\e[1;33m"
#define COLOR_BOLD_RED "\e[1;31m"
#define COLOR_RESET "\x1b[0m"

int block = 0; // variavel compartilhada entre as threads
int n; // numero de threads
int *vet;
pthread_mutex_t mutex;
pthread_cond_t cond;

void preenche_vetor(int *vet) {
    srand(time(NULL));
    for (int i = 0 ; i < n ; i++)
        vet[i] = rand() % 10; // 0 <= num <= 9
}

void imprime_vetor(int *vet, char msg[]) {
    printf("%s ", msg);
    printf("[  ");
    for (int i = 0 ; i < n ; i++)
        printf("%d  ", vet[i]);
    printf("]\n");
}

int gera_valor() {
    return rand() % 10;
}

void barreira() {
    pthread_mutex_lock(&mutex);
    if (block == n - 1) {
        pthread_cond_broadcast(&cond);
        block = 0;
    }
    else {
        block++;
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void *tarefa(void *arg) {
    // int id = * (int *) arg;
    int *s;
    s = (int *) malloc(sizeof(int));
    *s = 0;
    for (int i = 0 ; i < n ; i++) {
        *s += vet[i];
        barreira();
        vet[i] = gera_valor();
        barreira();
    }
    pthread_exit((void *) s);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf(COLOR_BOLD_YELLOW "\nExecute como: %s <numero de threads>\n\n" COLOR_RESET, argv[0]);
        return 1;
    }
    n = atoi(argv[1]);
    pthread_t tids_sistema[n];

    vet = (int *) malloc(n * sizeof(int));
    if (vet == NULL) {
        printf("Erro ao alocar o vetor\n");
        exit(1);
    }
    preenche_vetor(vet);
    imprime_vetor(vet, "Vetor inicial");

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // cria novas threads
    for (int i = 0 ; i < n ; i++) {
        if (pthread_create(&tids_sistema[i], NULL, tarefa, (void *) &i)) {
            printf("Erro na funcao pthread_create()\n");
            exit(1);
        }
    }

    // faz com que o fluxo principal espere o fim das threads criadas
	int s = 0;
    int valor_recebido[n];
    for (int i = 0 ; i < n ; i++) {
        int *ret;
		if (pthread_join(tids_sistema[i], (void **) &ret)) {
			printf("Erro na funcao pthread_join()\n");
			exit(1);
		}
        s += *ret;
        valor_recebido[i] = *ret;
        free(ret);
	}
    
    imprime_vetor(vet, "Vetor final  ");
    for (int i = 0 ; i < n ; i++)
        printf("Valor recebido da thread (%d): %d\n", i, valor_recebido[i]);

    if (s % n != 0) {
        printf(COLOR_BOLD_RED "Os valores nao foram somados corretamente\n\n" COLOR_RESET);
        exit(1);
    }
    printf(COLOR_BOLD_GREEN "Os valores foram somados corretamente\n\n" COLOR_RESET);
    
    free(vet);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}