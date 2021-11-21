#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define COLOR_BOLD_BLUE "\e[1;34m"
#define COLOR_BOLD_GREEN "\e[1;32m"
#define COLOR_BOLD_RED "\e[1;31m"
#define COLOR_RESET "\x1b[0m"

#define NTHREADS 2
#define TAM 10

typedef struct {
	int vetor[TAM];
	int inicio;
	int fim;
	int id_thread[NTHREADS]; // identificador local de cada thread
} Args;


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

void *eleva_quadrado(void *arg) {
	Args *elemento = (Args *) arg;
	int id = *elemento->id_thread;
	if (id == 1) {
		elemento->inicio = 0;
		elemento->fim = TAM / 2;
		// realiza as operacoes na primeira metade do vetor
		for (int i = elemento->inicio ; i <= elemento->fim ; i++)
			elemento->vetor[i] = elemento->vetor[i] * elemento->vetor[i];
	}
	else if (id == 2) {
		elemento->inicio = TAM / 2 + 1;
		elemento->fim = TAM;
		// realiza as operacoes na segunda metade do vetor
		for (int i = elemento->inicio ; i < elemento->fim ; i++)
			elemento->vetor[i] = elemento->vetor[i] * elemento->vetor[i];
	}

	pthread_exit(NULL);
}

int verifica_valores(int *vet) {
	for (int i = 0 ; i < TAM ; i++) {
		printf("eae vetor[i] = %d\n", vet[i]);
		if (vet[i] != i * i)
			return 0; // o vetor esta errado
	}
	return 1; // o vetor esta correto
}


int main() {
	pthread_t tids_sistema[NTHREADS]; // identificadores das threads no sistema
	// int id_thread[NTHREADS]; // identificador local de cada thread
	// int vetor[TAM];
	// preenche_vetor(vetor);
	Args elemento;
	preenche_vetor(elemento.vetor);
	
	for (int i = 0 ; i < NTHREADS ; i++) {
		elemento.id_thread[i] = i;
		if (pthread_create(&tids_sistema[i], NULL, eleva_quadrado, (void *) &elemento)) {
			printf(COLOR_BOLD_RED "Erro na funcao pthread_create()\n" COLOR_RESET);
			exit(1);
		}
	}

	if (verifica_valores(elemento.vetor))
		printf(COLOR_BOLD_BLUE "O vetor final esta correto\n" COLOR_RESET);
	else
		printf(COLOR_BOLD_RED "O vetor final esta errado\n" COLOR_RESET);

	pthread_exit(NULL); // desvincula o fim da main do fim das threads
	return 0;
}