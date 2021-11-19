#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define COLOR_BOLD_BLUE "\e[1;34m"
#define COLOR_BOLD_GREEN "\e[1;32m"
#define COLOR_BOLD_RED "\e[1;31m"
#define COLOR_RESET "\x1b[0m"

#define NTHREADS 2
#define TAM 10000

typedef struct {
	int vetor[TAM];
	int id_thread[NTHREADS]; // identificador local de cada thread
	int inicio;
	int fim;
} args;


void preenche_vetor(int *vet) {
	for (int i = 0 ; i < TAM ; i++)
		vet[i] = i;
}

void imprime_vetor(int *vet) {
	for (int i = 0 ; i < TAM ; i++) {
		printf("%d ", vet[i]);	
	}
	printf("\n");
}

void *eleva_quadrado(int *vet, int inicio, int fim, void *id_thread) {
	int id = * (int *) id_thread;
	if (id == 1) {
		for (int i = inicio ; i <= fim / 2 ; i++)
			vet[i] = vet[i] * vet[i];
	}
	else if (id == 2) {
		for (int i = fim / 2 + 1 ; i <= fim ; i++)
			vet[i] = vet[i] * vet[i];
	}

	pthread_exit(NULL);
}

int verifica_valores(int *vet) {
	for (int i = 0 ; i < TAM ; i++) {
		if (vet[i] != i * i)
			return 0; // o vetor esta errado
	}
	return 1; // o vetor esta correto
}


int main() {
	pthread_t tids_sistema[NTHREADS]; // identificadores das threads no sistema
	int id_thread[NTHREADS]; // identificador local de cada thread
	int vetor[TAM];
	preenche_vetor(vetor);
	
	for (int i = 0 ; i < NTHREADS ; i++) {
		id_thread[i] = i;
		if (pthread_create(&tids_sistema[i], NULL, eleva_quadrado, (void *) &id_thread[i])) {
			printf(COLOR_BOLD_RED "Erro na funcao pthread_create()\n" COLOR_RESET);
			exit(1);
		}
	}

	if (verifica_valores(vetor))
		printf(COLOR_BOLD_BLUE "O vetor final esta correto\n" COLOR_RESET);
	else
		printf(COLOR_BOLD_RED "O vetor final esta errado\n" COLOR_RESET);

	pthread_exit(NULL);
	return 0;
}