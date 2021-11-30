<h1 align="center">
  Laboratório 2
</h1>

## O código foi compilado com o comando `gcc mat.c -o mat -Wall -lpthread`

### Tabelas de desempenho

#### Matriz 500x500

Número de threads | Tempo médio sequencial | Tempo médio concorrente | Aceleração média |
:---------------: | :--------------------: | :---------------------: | :--------------: |
1 | 1.20063 | 1.20136 | 0.99939 |
2 | 1.10589 | 0.50123 | 2.20635 |
4 | 1.08690 | 0.31627 | 3.43662 |

#### Matriz 1000x1000

Número de threads | Tempo médio sequencial | Tempo médio concorrente | Aceleração média |
:---------------: | :--------------------: | :---------------------: | :--------------: |
1 | 9.33183 | 10.21354 | 0.91367 |
2 | 9.28184 | 4.67665  | 1.98472 |
4 | 9.28661 | 2.84496  | 3.26423 |

#### Matriz 2000x2000

Número de threads | Tempo médio sequencial | Tempo médio concorrente | Aceleração média |
:---------------: | :--------------------: | :---------------------: | :--------------: |
1 | 97.70511 | 100.59556 | 0.97126 |
2 | 99.24009 | 51.64913  | 1.92143 |
4 | 99.60588 | 35.19936  | 2.82976 |
