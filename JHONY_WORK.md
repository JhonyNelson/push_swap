# Guia da Parte do Jhony — push_swap

Este documento explica as responsabilidades do Jhony no projeto, o que já está
presente no código e o que será feito nas próximas etapas. O objetivo é permitir
que Guilherme entenda e revise essa parte da dupla.

> Estado atual: as operações de rotação estão implementadas. `compute_disorder`,
> `sort_5`, flags, `adaptive_sort`, a recuperação do `medium_sort` e os testes
> finais ainda dependem da implementação e integração do Jhony.

## Visão geral

No `push_swap`, A e B são listas duplamente encadeadas. Cada nó possui:

```text
value  -> número original
index  -> posição do número na ordem crescente
prev   -> nó anterior
next   -> próximo nó
```

As funções do Jhony lidam principalmente com rotações, análise da entrada,
decisão adaptativa e qualidade final do projeto.

## Arquivos da parte do Jhony

| Arquivo | Status | Responsabilidade |
|---|---|---|
| `src/operations/rotate.c` | Implementado | `ra`, `rb` e `rr`. |
| `src/operations/reverse_rotate.c` | Implementado | `rra`, `rrb` e `rrr`. |
| `src/metrics/disorder.c` | Planejado | `compute_disorder`. |
| `src/sorting/small_sort.c` | Planejado | `sort_5` será incluído neste módulo ou em um auxiliar próprio. |
| `src/parsing/flags.c` | Planejado | `parse_flags`. |
| `src/sorting/medium_sort.c` | Planejado | Recuperação de B e otimização de rotações. |
| `src/sorting/adaptive_sort.c` | Planejado | Escolha de estratégia por disorder. |
| testes e documentação | Planejado | Casos extremos, Valgrind, Norminette e revisão final. |

---

## Dia 2 — Rotações

### `ft_rotate`, `ra`, `rb` e `rr`

Uma rotação move o primeiro nó para o final da mesma stack.

```text
Antes: A = 1 2 3 4
ra
Depois: A = 2 3 4 1
```

`ft_rotate` realiza a alteração dos ponteiros:

1. Guarda o primeiro nó e encontra o último.
2. O segundo nó vira o novo topo e recebe `prev = NULL`.
3. O antigo último nó passa a apontar para o antigo primeiro.
4. O antigo primeiro passa a apontar de volta para o antigo último.
5. O antigo primeiro deixa de ter `next`, pois agora é o último.

As funções públicas são:

| Função | Efeito | Saída no `stdout` |
|---|---|---|
| `ra(&a)` | roda A para cima | `ra` |
| `rb(&b)` | roda B para cima | `rb` |
| `rr(&a, &b)` | roda A e B ao mesmo tempo | `rr` |

Com zero ou um nó, a rotação não altera a stack.

### `ft_rev_rotate`, `rra`, `rrb` e `rrr`

A rotação reversa move o último nó para o topo.

```text
Antes: A = 1 2 3 4
rra
Depois: A = 4 1 2 3
```

`ft_rev_rotate` faz o caminho contrário da rotação comum:

1. Encontra o último nó e o penúltimo.
2. O penúltimo passa a ser o novo último, com `next = NULL`.
3. O antigo último perde seu `prev`.
4. O antigo último passa a apontar para o antigo topo.
5. O antigo último vira o novo topo, com `prev = NULL`.

As funções públicas são:

| Função | Efeito | Saída no `stdout` |
|---|---|---|
| `rra(&a)` | roda A para baixo | `rra` |
| `rrb(&b)` | roda B para baixo | `rrb` |
| `rrr(&a, &b)` | roda A e B para baixo ao mesmo tempo | `rrr` |

---

## Dia 3 — Métrica de desordem e `sort_5`

### `compute_disorder` (planejado)

Disorder mede quanto a stack A está fora de ordem. O valor final precisa ficar
entre `0` e `1`.

Para calcular:

1. Compare cada número com todos os que vêm depois dele.
2. Quando um número maior aparece antes de um menor, conte um erro.
3. Divida o número de erros pela quantidade total de pares comparados.

```text
A: 3 1 2

Pares: (3,1), (3,2), (1,2)
Erros: (3,1), (3,2) = 2
Total de pares: 3
disorder = 2 / 3 = 0.66
```

Uma stack ordenada tem disorder `0`; uma stack em ordem inversa tem disorder
`1`. Para zero ou um elemento, a função deve retornar `0` para evitar divisão
por zero.

Uso futuro:

```c
disorder = compute_disorder(stack_a);
```

### `sort_5` (planejado)

`sort_5` ordena até cinco números usando as operações básicas e `sort_3`.

Uma estratégia simples é:

1. Localizar o menor número em A e levá-lo ao topo com `ra` ou `rra`.
2. Executar `pb`.
3. Repetir até restarem três números em A.
4. Chamar `sort_3(&a)`.
5. Executar `pa` até B voltar para A.

Exemplo:

```text
A: 5 1 4 2 3
pb dos menores -> A fica com 5 4 3 | B guarda 2 1
sort_3          -> A fica com 3 4 5
pa, pa          -> A fica com 1 2 3 4 5
```

---

## Dia 4 — Flags de linha de comando

### `parse_flags` (planejado)

O subject pede quatro seletores de estratégia e um modo de benchmark:

| Flag | Efeito |
|---|---|
| `--simple` | força a estratégia `O(n²)` |
| `--medium` | força a estratégia `O(n√n)` |
| `--complex` | força a estratégia `O(n log n)` |
| `--adaptive` | escolhe automaticamente pela desordem; é o padrão |
| `--bench` | ativa métricas no `stderr` depois da ordenação |

`parse_flags` precisa separar flags dos números, escolher uma estratégia padrão
quando não houver seletor e rejeitar uma flag desconhecida como erro. A saída
das operações continua no `stdout`; benchmark e erros vão para `stderr`.

---

## Dia 5 — Recuperação de B no `medium_sort`

O `medium_sort` divide os índices em faixas, chamadas *chunks*. A parte do
Guilherme envia os índices de cada faixa de A para B. A parte do Jhony recebe B
e precisa devolvê-la para A em ordem.

O contrato compartilhado definido no header é:

```c
int	medium_chunk_size(int size);
void	medium_push_chunks(t_stack **a, t_stack **b, int chunk_size);
void	medium_restore(t_stack **a, t_stack **b);
```

`medium_chunk_size` calcula `ceil(sqrt(n))` sem usar a biblioteca matemática.
`medium_push_chunks` já foi implementada pelo Guilherme. Ela espera índices
calculados por `assign_index`, recebe o tamanho do chunk e deixa A vazia. Ao
enviar a metade menor de cada chunk, ela aplica `rb` para manter esses valores
mais abaixo em B. O Jhony deve implementar `medium_restore`, consumindo B até
ela ficar vazia e deixando A ordenada.

Uma abordagem comum:

1. Localizar o maior índice presente em B.
2. Escolher `rb` ou `rrb` pelo caminho mais curto para levá-lo ao topo.
3. Executar `pa`.
4. Repetir até B ficar vazia.

Como os maiores valores retornam primeiro, cada novo `pa` fica acima do valor
anterior e A termina em ordem crescente.

Também é a etapa para procurar oportunidades de usar `rr` e `rrr`, quando uma
rotação necessária em A e outra em B têm a mesma direção.

---

## Dia 6 — `adaptive_sort`

`adaptive_sort` usa o resultado de `compute_disorder` para escolher uma das
estratégias já existentes:

```text
disorder < 0.2            -> simple_sort
0.2 <= disorder < 0.5     -> medium_sort
disorder >= 0.5           -> complex_sort
```

Ela não precisa ordenar por conta própria. Sua responsabilidade é medir a
entrada, aplicar os thresholds exigidos pelo subject e chamar o algoritmo certo.

Uso futuro:

```c
disorder = compute_disorder(stack_a);
adaptive_sort(&stack_a, &stack_b, disorder);
```

---

## Dia 7 — Qualidade e testes

O Jhony é responsável por liderar a validação final. Casos importantes:

- entrada vazia e um valor;
- stack já ordenada e ordem inversa;
- valores negativos;
- `INT_MIN` e `INT_MAX`;
- overflow e underflow;
- duplicados;
- caracteres inválidos e strings vazias;
- entradas grandes, especialmente 100 e 500 números;
- cada estratégia e o modo `--bench`.

Além disso, o projeto deve passar por Valgrind, Norminette e pelos checkers
fornecidos. O checker recebe operações pelo `stdin` e responde `OK` apenas se
A terminar ordenada e B vazia.

## Como essa parte entra no programa final

```text
argv
  -> parse_flags
  -> validação
  -> criação de A e B
  -> assign_index
  -> compute_disorder
  -> estratégia escolhida ou adaptive_sort
  -> operações no stdout
  -> --bench no stderr, se solicitado
  -> liberar memória
```

O `main` será integrado em conjunto. Ele precisa usar as funções de ambos os
participantes, sem imprimir valores de debug no `stdout`.

## O que revisar ao receber código do Jhony

- As rotações preservam todos os `prev` e `next`?
- As operações não causam segfault com zero ou um nó?
- `compute_disorder` retorna de `0` a `1` e evita divisão por zero?
- `sort_5` deixa B vazia e A ordenada?
- As flags não são confundidas com números?
- `--bench` nunca mistura métricas com as operações do `stdout`?
- Não há variáveis globais, vazamentos ou erros de Norminette?
