<h1 align="center">
  Laboratório 3
</h1>

## O código foi compilado com o comando `gcc lab3.c -o lab3 -Wall -lpthread`

### Tabelas de desempenho

#### Dimensão 10⁵

Número de threads | Tempo sequencial | Tempo concorrente | Aceleração média |
:---------------: | :--------------: | :---------------: | :--------------: |
1 | 0.00093 | 0.00160 | - |
2 | 0.00072 | 0.00128 | - |
4 | 0.00077 | 0.00103 | - |

#### Dimensão 10⁷

Número de threads | Tempo sequencial | Tempo concorrente | Aceleração média |
:---------------: | :--------------------: | :---------------------: | :--------------: |
1 | 0.08332 | 0.07961 | - |
2 | 0.08344 | 0.04011 | - |
4 | 0.07902 | 0.02132 | - |

#### Dimensão 10⁸

Número de threads | Tempo sequencial | Tempo concorrente | Aceleração média |
:---------------: | :--------------------: | :---------------------: | :--------------: |
1 | 0.84182 | 0.79739 | - |
2 | 0.83707 | 0.39675 | - |
4 | 0.83734 | 0.24600 | - |
