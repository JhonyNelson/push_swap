# Guia da Parte do Guilherme — push_swap

Este documento explica as partes implementadas por Guilherme no projeto,
usando linguagem simples. Ele também mostra como essas partes se encaixam no
programa final.

> Estado atual: as funções abaixo compilam e foram testadas isoladamente. O
> `main` ainda não escolhe nem chama os algoritmos; essa integração depende das
> flags, da métrica de desordem e do trabalho conjunto do Dia 4.

## Visão geral

No `push_swap`, temos duas stacks:

```text
A: 4 2 1
B: 3
```

O primeiro número é sempre o topo. O objetivo é terminar com todos os valores
em A, em ordem crescente, e B vazia:

```text
A: 1 2 3 4
B: vazia
```

Os algoritmos nunca devem mexer diretamente em `next` e `prev`. Eles usam as
operações permitidas (`sa`, `pb`, `ra`, etc.), que cuidam dos ponteiros.

## Arquivos da sua parte

| Arquivo | Responsabilidade |
|---|---|
| `src/operations/swap.c` | Troca os dois primeiros valores de uma stack. |
| `src/operations/push.c` | Move o nó do topo de uma stack para a outra. |
| `src/metrics/index.c` | Calcula o índice ordenado de cada valor. |
| `src/sorting/small_sort.c` | Ordena stacks com até três valores. |
| `src/sorting/simple_sort.c` | Estratégia simples, de complexidade `O(n²)`. |
| `src/sorting/complex_sort.c` | Estratégia Radix LSD, de complexidade `O(n log n)`. |

O `Makefile` já compila todos esses arquivos.

---

## Dia 2 — Operações `swap` e `push`

### `ft_swap`, `sa`, `sb` e `ss`

`ft_swap` troca os valores dos dois primeiros nós de uma stack. Ela não muda
os nós de lugar e não altera ponteiros; troca somente `value`.

```text
Antes: A = 3 1 2
sa
Depois: A = 1 3 2
```

As funções públicas são:

| Função | Efeito | Saída no `stdout` |
|---|---|---|
| `sa(&a)` | troca o topo de A | `sa` |
| `sb(&b)` | troca o topo de B | `sb` |
| `ss(&a, &b)` | troca o topo de A e B | `ss` |

Com zero ou um nó, a troca não altera a stack.

### `ft_push`, `pa` e `pb`

`ft_push` move o primeiro **nó** da origem para o topo do destino. Não cria e
nem libera memória.

```text
Antes do pb:
A: 4 2 1
B: 3

Depois do pb:
A: 2 1
B: 4 3
```

Passos internos:

1. Guarda o topo da origem.
2. Atualiza o topo da origem para o próximo nó.
3. Desconecta o nó removido, zerando `next` e `prev`.
4. Insere esse nó no topo da stack de destino com `ft_stackadd_front`.

Zerar os ponteiros antes da inserção evita que o nó continue ligado à antiga
stack, especialmente quando a stack de destino está vazia.

| Função | Movimento | Saída no `stdout` |
|---|---|---|
| `pa(&a, &b)` | topo de B vai para A | `pa` |
| `pb(&a, &b)` | topo de A vai para B | `pb` |

Se a stack de origem estiver vazia, nenhum nó é movido.

---

## Dia 3 — Índices e ordenação de três valores

### `assign_index`

Os valores originais podem ser negativos, como `-10`. Para o Radix Sort, é mais
simples trabalhar com índices entre `0` e `n - 1`.

```text
Valores:  42  -10   7  100   0
Índices:   3    0   2    4   1
```

Para cada nó, `assign_index` conta quantos valores são menores que ele. Esse
total vira `node->index`.

```text
Valor 42: existem -10, 7 e 0 abaixo dele na ordem crescente.
Resultado: index = 3.
```

A função não muda a ordem de A, não modifica `value` e não cria nós. Sua
complexidade é `O(n²)`, mas ela roda apenas uma vez antes da ordenação.

Uso na integração final:

```c
assign_index(&stack_a);
```

### `sort_3`

`sort_3` ordena uma stack com dois ou três valores. Para duas posições, aplica
`sa` se necessário. Para três, ela primeiro move o maior valor para o fim com
`ra` ou `rra` e, depois, aplica `sa` se os dois primeiros ainda estiverem fora
de ordem.

Exemplo:

```text
A: 3 2 1
ra  -> 2 1 3
sa  -> 1 2 3
```

Ela é usada pelos algoritmos maiores para resolver o final de uma stack pequena.

Uso:

```c
sort_3(&stack_a);
```

---

## Dia 4 — `simple_sort`

`simple_sort` é a estratégia básica de complexidade `O(n²)`. Ela é fácil de
entender e serve como referência para comparar com os algoritmos mais rápidos.

Fluxo:

1. Encontra a posição do menor valor em A.
2. Escolhe `ra` ou `rra`, usando o caminho mais curto até o topo.
3. Executa `pb` e deixa esse menor valor em B.
4. Repete até restarem três valores em A.
5. Usa `sort_3` para ordenar os três restantes.
6. Executa `pa` até B voltar inteira para A.

Exemplo simplificado:

```text
A: 4 1 3 2
B: vazia

ra, pb        -> A: 3 2 4 | B: 1
sort_3        -> A: 2 3 4 | B: 1
pa            -> A: 1 2 3 4 | B: vazia
```

Uso na integração final:

```c
simple_sort(&stack_a, &stack_b);
```

---

## Dia 6 adiantado — `complex_sort`

`complex_sort` usa Radix Sort LSD (*Least Significant Digit*). Em vez de olhar
os valores originais, olha um bit por vez de `index`.

Para cada bit:

- bit igual a `0`: envia o nó para B com `pb`;
- bit igual a `1`: mantém o nó em A, movendo-o ao fim com `ra`;
- ao terminar o bit: traz tudo de B de volta para A com `pa`.

Exemplo com índices de `0` a `4`:

```text
Índices em binário:
0 = 000
1 = 001
2 = 010
3 = 011
4 = 100
```

O algoritmo organiza primeiro o bit menos significativo, depois o próximo, até
o bit mais significativo. Ao final, os índices — e consequentemente os valores
originais — ficam em ordem crescente.

`complex_sort` exige que `assign_index` tenha sido chamado antes:

```c
assign_index(&stack_a);
complex_sort(&stack_a, &stack_b);
```

No modelo de operações do `push_swap`, cada bit processa todos os elementos e
o número de bits cresce com `log n`; por isso a estratégia é `O(n log n)`.

---

## Como compilar e testar

Compile o projeto na raiz:

```bash
make
```

As funções de ordenação foram testadas isoladamente. O executável atual ainda
não chama `simple_sort` nem `complex_sort` pelo `main`; portanto, ele ainda não
deve ser usado com o checker para validar os algoritmos completos.

Quando o `main` estiver integrado, o fluxo será:

```text
argv
  -> validação e flags
  -> criação de A e B
  -> assign_index
  -> algoritmo selecionado
  -> operações no stdout
  -> checker
```

Exemplo da futura validação:

```bash
ARG="4 67 3 87 23"
./push_swap --complex $ARG | /caminho/para/checker_linux $ARG
```

O checker deve responder `OK`.

## O que ainda falta

### Responsabilidades do Guilherme

- Implementar a parte principal de `medium_sort` (chunks e envio de A para B).
- Implementar o benchmark `--bench` após a integração do `main`.

### Responsabilidades do Jhony

- `compute_disorder`.
- `sort_5`.
- `parse_flags`.
- Parte de recuperação e otimização de B no `medium_sort`.
- `adaptive_sort`.
- Testes extremos, Valgrind e Norminette.

### Trabalho conjunto

- Integrar as flags, as estratégias e o benchmark no `main`.
- Validar o programa com os checkers fornecidos.
- Revisar parsing, operações, memória e número de operações.
