import sys
import subprocess
import time

if (len(sys.argv) < 2) :
    print("Execute como: python3 <{}> <tipo do vetor>".format(sys.argv[0]))
    print("Tipo 1: Vetor aleatorio\nTipo 2: Vetor descrescente")
    print("Tipo 3: Vetor ordenado")
    sys.exit(0)

valores = [1, 2, 4, 8, 16, 32] # dimensoes do vetor (em milhoes)
threads = [2, 3, 4]
processos = []
tipo_vetor = sys.argv[1]

inicio = time.time()
with open("saida.txt", "w") as f :
    for valor in valores :
        val = valor * 1000000
        for thread in threads :
            processos.append(subprocess.run(
                ["./quicksort", str(val), str(thread), str(tipo_vetor)], stdout=f))

for processo in processos :
    fim = time.time()
    if (processo.returncode > 0) :
        print("Falha nos testes")
        print("Tempo de execução dos testes: {:.5f} seg".format(fim - inicio))
        sys.exit(1)
fim = time.time()
print("Todos os testes passaram")
print("Tempo de execução dos testes: {:.5f} seg".format(fim - inicio))

# testar com vetores ordenados e decrescentes