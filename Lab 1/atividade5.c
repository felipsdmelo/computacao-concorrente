#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define COLOR_BOLD_BLUE "\e[1;34m"
#define COLOR_BOLD_GREEN "\e[1;32m"
#define COLOR_BOLD_RED "\e[1;31m"
#define COLOR_RESET "\x1b[0m"

#define NTHREADS 2
#define TAM 10000


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

void eleva_quadrado(int *vet, int inicio, int fim) {
	for (int i = inicio ; i <= fim ; i++)
		vet[i] = vet[i] * vet[i];
}

int verifica_valores(int *vet) {
	for (int i = 0 ; i < TAM ; i++) {
		if (vet[i] != i * i)
			return 0; // o vetor esta errado
	}
	return 1; // o vetor esta correto
}


int main() {
	printf("\n");
	pthread_t tids_sistema[NTHREADS]; // identificadores das threads no sistema
	int id_thread[NTHREADS]; // identificador local de cada thread
	int vetor[TAM];
	preenche_vetor(vetor);
	eleva_quadrado(vetor, 0, TAM - 1);

	if (verifica_valores(vetor))
		printf(COLOR_BOLD_BLUE "O vetor final esta correto\n\n" COLOR_RESET);
	else
		printf(COLOR_BOLD_RED "O vetor final esta errado\n\n" COLOR_RESET);

	
	return 0;
}