import sys
import subprocess
import time

if (len(sys.argv) < 2) :
    print("Execute como: python3 <{}> <quantidade de testes>".format(sys.argv[0]))
    sys.exit(0)

processos = []
qtd = int(sys.argv[1])

inicio = time.time()
with open("saida.txt", "w") as f :
    for i in range(qtd) :
        processos.append(subprocess.run(["./lab9"], stdout=f))
fim = time.time()
print("Tempo de execução dos testes: {:.5f} seg".format(fim - inicio))
