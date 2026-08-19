# Guia de Fluxo e Estudo — push_swap

Este guia mostra o caminho completo de uma execução e a responsabilidade de
cada camada. Leia-o junto com o código, seguindo a ordem sugerida ao final.

## Visão geral

```text
argv
  |
  v
main
  |
  +--> parse_flags
  |
  +--> init_stack_from_args
  |      |
  |      +--> normalize_args
  |      +--> validate_numbers
  |      +--> init_stack / ft_new_node
  |      +--> ft_check_duplicates
  |
  +--> compute_disorder
  |
  +--> execute_strategy
  |      |
  |      +--> assign_index
  |      +--> simple_sort / medium_sort / complex_sort
  |               |
  |               +--> sa, pb, ra, rra, ...
  |                       |
  |                       +--> ft_swap, ft_push, ft_rotate, ...
  |                       +--> emit_operation
  |
  +--> print_benchmark (somente com --bench)
  |
  +--> ft_freestack
```

`main.c` não implementa algoritmo nem manipula `next` ou `prev`. Ele apenas
coordena as etapas, trata erro e libera recursos ao final.

## 1. Entrada e flags

Exemplo:

```bash
./push_swap --bench --adaptive "4 67" 3 87 23
```

`parse_flags` em `src/parsing/flags.c` começa em `argv[1]` e aceita flags
somente antes dos números. Ela devolve o índice do primeiro argumento numérico
e preenche duas informações no contexto de métricas:

```text
metrics.strategy = STRATEGY_ADAPTIVE
metrics.bench    = 1
```

Se não existir seletor, `--adaptive` é o padrão. Um seletor desconhecido ou
mais de uma estratégia gera `Error`.

## 2. Normalização, validação e criação de A

`init_stack_from_args` é a porta de entrada da camada de stack. Ela chama as
etapas de parsing antes de criar nós:

```text
"4 67" + "3" + "87" + "23"
             |
             v
       normalize_args
             |
             v
["4", "67", "3", "87", "23"]
             |
             v
      validate_numbers
             |
             v
          init_stack
```

`normalize_args` usa `ft_split` para suportar argumentos com aspas e combina
tudo em uma única lista. `validate_numbers` verifica sintaxe, sinal, overflow
e underflow. Depois, `ft_check_duplicates` impede valores repetidos.

Por fim, `init_stack` chama `ft_new_node` para cada inteiro e monta A como
lista duplamente ligada.

## 3. O contexto `t_metrics`

O `main` cria uma variável local `t_metrics metrics` e a zera com `ft_bzero`.
Não existe variável global.

Cada nó criado recebe o ponteiro `metrics`. Assim, quando uma operação é
executada, ela consegue atualizar o benchmark sem precisar receber muitos
parâmetros:

```text
main local: metrics
                  ^
                  |
node A <----------+
node A <----------+
node B <----------+
```

O contexto guarda:

- a desordem inicial;
- a estratégia escolhida;
- se `--bench` foi solicitado;
- o total de operações;
- a contagem individual de `sa`, `pb`, `ra` e todas as demais.

## 4. Disorder e índices

Antes de qualquer movimento, `compute_disorder` mede pares fora de ordem:

```text
A: 3 1 2

Pares: (3,1), (3,2), (1,2)
Erros: (3,1), (3,2) = 2
Disorder: 2 / 3 = 0,666...
```

Se a stack não estiver ordenada, `execute_strategy` chama `assign_index`.
Cada valor recebe sua posição na ordem crescente, entre `0` e `n - 1`:

```text
Valor:  42  -10   7  100   0
Índice:  3    0   2    4   1
```

Os índices facilitam o Radix Sort e o algoritmo por blocos, inclusive para
números negativos.

## 5. Escolha da estratégia

`execute_strategy` seleciona diretamente a flag solicitada. Em modo adaptativo,
`adaptive_strategy` usa os thresholds exigidos pelo subject:

| Disorder | Estratégia | Complexidade declarada |
|---|---|---|
| `< 0,20` | `simple_sort` | `O(n²)` |
| `0,20` até `< 0,50` | `medium_sort` | `O(n√n)` |
| `≥ 0,50` | `complex_sort` | `O(n log n)` |

Se A já estiver ordenada, nenhuma operação é impressa. No benchmark adaptativo,
a estratégia ainda é identificada de acordo com a desordem.

## 6. Algoritmos

### `simple_sort`

Arquivo: `src/sorting/simple_sort.c`.

1. Encontra o menor valor de A.
2. Escolhe `ra` ou `rra` para trazê-lo ao topo pelo caminho mais curto.
3. Executa `pb`.
4. Repete até restarem três valores.
5. Usa `sort_3`.
6. Usa `pa` para reconstruir A crescente.

É simples de entender, mas cresce rapidamente para entradas grandes.

### `medium_sort`

Arquivos: `medium_blocks.c` e `medium_restore.c`.

1. Calcula um tamanho de bloco próximo de `sqrt(n)`.
2. Envia índices de cada bloco de A para B.
3. Rotaciona B quando o índice está na metade inferior do bloco, deixando a
   recuperação mais barata.
4. Recupera os maiores índices de B com `pa`.

É o equilíbrio entre simplicidade e desempenho.

### `complex_sort`

Arquivo: `complex_sort.c`.

É um Radix Sort LSD baseado nos índices:

```text
bit = 0, 1, 2, ...

bit do índice = 0  -> pb
bit do índice = 1  -> ra
fim de cada bit    -> pa até B ficar vazia
```

É a estratégia mais previsível para entradas muito desordenadas.

## 7. Operações e saída

Os algoritmos nunca mudam ponteiros diretamente. Eles usam apenas operações
permitidas, como `pb`, `ra` e `rra`.

Cada operação pública segue este padrão:

```text
1. ft_push / ft_rotate / ft_swap altera a lista.
2. A operação informa se realmente conseguiu agir.
3. emit_operation escreve a instrução no stdout e atualiza o contador.
```

Isso evita imprimir comandos inúteis em stacks vazias ou com apenas um nó.

## 8. Benchmark

Use:

```bash
./push_swap --bench --adaptive 4 67 3 87 23
```

As operações vão para `stdout`; o relatório vai para `stderr`. Para verificar
com o checker sem misturar as saídas:

```bash
ARG="4 67 3 87 23"
./push_swap --bench $ARG 2>benchmark.txt | ./checker_linux $ARG
cat benchmark.txt
```

Exemplo de relatório:

```text
Disorder: 50.00%
Strategy: complex (O(n log n))
Operations: 42
sa: 0
...
rrr: 0
```

## Ordem recomendada para estudar

1. `include/push_swap.h`: tipos, enums e contratos públicos.
2. `src/main.c`: visão completa do programa.
3. `src/parsing/flags.c`, `input_normalization.c` e `input_validation.c`.
4. `src/stack/*.c`: nós, lista duplamente ligada e limpeza.
5. `src/operations/*.c`: regras reais de `sa`, `pb`, `ra` e afins.
6. `src/metrics/index.c` e `disorder.c`.
7. `src/sorting/small_sort.c` e `simple_sort.c`.
8. `src/sorting/medium_*.c` e `complex_sort.c`.
9. `src/sorting/strategy.c` e `src/metrics/benchmark.c`.

Ao estudar cada função, responda a estas três perguntas:

1. Qual é a pré-condição dela?
2. O que ela muda em A, B ou nas métricas?
3. Quem chama essa função e qual será o próximo passo do fluxo?
