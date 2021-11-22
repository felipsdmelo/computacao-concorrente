<h1 align="center">
  Laboratório 1
</h1>

## Todos os códigos foram compilados com o comando `gcc arquivo.c -o arquivo -Wall -lpthread`

## Atividade 1

Provavelmente sim. O que não muda em cada execução é a ordem dos printf() relacionados à criação de novas threads. Isso ocorre pois essa função está na main e, portanto, é executada de maneira sequencial

## Atividade 2

A diferença para o programa anterior é que agora, por haver um parâmetro representando o identificador local de cada thread, que é impresso, podemos ver que a ordem de execução de alguns printf() das threads muda

## Atividade 3

O programa funcionou como esperado. As funções printf() da main são impressas de forma sequencial, enquanto as da função são impressas em ordens diferentes

## Atividade 4

A thread principal sempre é a última a terminar, pois aguarda o término das threads que foram criadas
