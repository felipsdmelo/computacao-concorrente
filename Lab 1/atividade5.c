#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define COLOR_BOLD_BLUE "\e[1;34m"
#define COLOR_BOLD_GREEN "\e[1;32m"
#define COLOR_BOLD_RED "\e[1;31m"
#define COLOR_RESET "\x1b[0m"

#define NTHREADS 2
#define TAM 10000

int vetor[TAM]; // variavel global do vetor

void preenche_vetor(int *vet) {
	for (int i = 0 ; i < TAM ; i++)
		vet[i] = i;
}

void *eleva_quadrado(void *arg) {
	int id = * (int *) arg;
	
	// realiza as operacoes na primeira metade do vetor
	if (id == 1) {
		int inicio = 0;
		int fim = TAM / 2; // armazena a parte inteira do quociente
		for (int i = inicio ; i <= fim ; i++) {
			vetor[i] = vetor[i] * vetor[i];
		}
	}
	// realiza as operacoes na segunda metade do vetor
	else if (id == 2) {
		int inicio = TAM / 2 + 1;
		int fim = TAM;
		for (int i = inicio ; i < fim ; i++) {
			vetor[i] = vetor[i] * vetor[i];
		}
	}
	pthread_exit(NULL);
}

int verifica_valores(int *vet) {
	for (int i = 0 ; i < TAM ; i++) {
		if (vet[i] != i * i) {
			printf("Erro na posicao %d   |   Valor atual: %d   |   Valor esperado: %d\n", i, vet[i], i * i);
			return 0; // o vetor esta errado
		}
	}
	return 1; // o vetor esta correto
}


int main() {
	pthread_t tids_sistema[NTHREADS]; // identificadores das threads no sistema
	int id_thread[NTHREADS]; // identificador local de cada thread
	preenche_vetor(vetor);
	
	for (int i = 0 ; i < NTHREADS ; i++) {
		id_thread[i] = i + 1;

		// cria nova thread
		if (pthread_create(&tids_sistema[i], NULL, eleva_quadrado, (void *) &id_thread[i])) {
			printf(COLOR_BOLD_RED "Erro na funcao pthread_create()\n" COLOR_RESET);
			exit(1);
		}
	}

	// faz com que o fluxo principal espere o fim das threads criadas
	for (int i = 0 ; i < NTHREADS ; i++) {
		if (pthread_join(tids_sistema[i], NULL)) {
			printf(COLOR_BOLD_RED "Erro na funcao pthread_join()\n" COLOR_RESET);
			exit(1);
		}
	}

	if (verifica_valores(vetor))
		printf(COLOR_BOLD_BLUE "O vetor final esta correto\n" COLOR_RESET);
	else
		printf(COLOR_BOLD_RED "O vetor final esta errado\n" COLOR_RESET);

	pthread_exit(NULL); // desvincula o fim da main do fim das threads
	return 0;
}