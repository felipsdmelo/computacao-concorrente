<h1 align="center">
  Laboratório 3
</h1>

## O código foi compilado com o comando `gcc lab3.c -o lab3 -Wall -lpthread`

#### Foram utilizados números aleatórios de 0 a 1000 no vetor e a faixa de valores testados foi 0 < x < 500

### Tabelas de desempenho

#### Dimensão 10⁵

Número de threads | Tempo sequencial | Tempo concorrente | Aceleração média |
:---------------: | :--------------: | :---------------: | :--------------: |
1 | 0.00093 | 0.00160 | 0.58125 |
2 | 0.00072 | 0.00128 | 0.56250 |
4 | 0.00077 | 0.00103 | 0.74757 |

#### Dimensão 10⁷

Número de threads | Tempo sequencial | Tempo concorrente | Aceleração média |
:---------------: | :--------------------: | :---------------------: | :--------------: |
1 | 0.08332 | 0.07961 | 1.04660 |
2 | 0.08344 | 0.04011 | 2.08027 |
4 | 0.07902 | 0.02132 | 3.70638 |

#### Dimensão 10⁸

Número de threads | Tempo sequencial | Tempo concorrente | Aceleração média |
:---------------: | :--------------------: | :---------------------: | :--------------: |
1 | 0.84182 | 0.79739 | 1.05572 |
2 | 0.83707 | 0.39675 | 2.10981 |
4 | 0.83734 | 0.24600 | 3.40382 |
