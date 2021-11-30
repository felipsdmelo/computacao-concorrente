<h1 align="center">
  Laboratório 2
</h1>

## O código foi compilado com o comando `gcc mat.c -o mat -Wall -lpthread`

### Tabelas de desempenho

#### Matriz 500x500

|   Número de threads   |  Tempo médio sequencial   |   Tempo médio concorrente   |   Aceleração média   |
|    :---:              |     :---:                 |     :---:                   |                      |
| 1                     | 1.20063                   | 10.21354                    |                      |
| 2                     | 1.10589                   | 4.67665                     |                      |
| 4                     | 1.08690                   | 2.84496                     |                      |

#### Matriz 1000x1000

|   Número de threads   |  Tempo médio sequencial   |   Tempo médio concorrente   |   Aceleração média   |
|    :---:              |     :---:                 |     :---:                   |                      |
| 1                     | 9.33183                   | 1.20136                     |                      |
| 2                     | 9.28184                   | 0.50123                     |                      |
| 4                     | 9.28661                   | 0.31627                     |                      |

#### Matriz 2000x2000

|   Número de threads   |  Tempo médio sequencial   |   Tempo médio concorrente   |   Aceleração média   |
|    :---:              |     :---:                 |     :---:                   |                      |
| 1                     |                           |                             |                      |
| 2                     |                           |                             |                      |
| 4                     |                           |                             |                      |
