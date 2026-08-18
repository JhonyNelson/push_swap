# 🗺️ Roadmap Cronológico Push_swap v1.1 — 7 Dias

## 🟩 Dia 1: A Fundação (Concluído ✅)

*O objetivo foi ter um programa que compila, recebe números, valida e guarda na memória.*

### Trabalho conjunto

- Makefile e Libft linkada.
- Struct `t_stack` (Lista Duplamente Ligada).
- Funções de manipulação base:
  - `ft_new_node`
  - `ft_add_back`
  - `ft_stacklast`
  - `ft_stackadd_front`
  - `ft_stacksize`
  - `ft_freestack`
- Parsing e validação de erros:
  - `ft_isdigit_str`
  - `ft_check_args`
  - `init_stack`
- `push_swap.h` com todos os prototypes.

---

## 🟨 Dia 2: As Regras do Jogo

*Construir todas as operações fundamentais das stacks.*

### Guilherme — Swap e Push

- `ft_swap`
- `sa`
- `sb`
- `ss`
- `ft_push`
- `pa`
- `pb`

### Jhony — Rotate e Reverse Rotate

- `ft_rotate`
- `ra`
- `rb`
- `rr`
- `ft_rev_rotate`
- `rra`
- `rrb`
- `rrr`

### Trabalho conjunto

- Testar todas as operações.
- Testar stacks com:
  - 0 elementos
  - 1 elemento
  - 2 elementos
  - vários elementos
- Garantir que nenhuma operação gera `segfault`.

### Meta

Ao final do dia, todas as operações básicas devem estar funcionando corretamente e serem independentes dos algoritmos de ordenação.

---

## 🟧 Dia 3: Infraestrutura de Ordenação

*Preparar a estrutura necessária para todos os algoritmos.*

### Guilherme

- `assign_index`
  - Transformar os valores em índices `0, 1, 2...`.
  - Garantir que o menor valor receba `0`.
  - Garantir que o maior receba `n - 1`.
- `sort_2`
- `sort_3`

### Jhony

- `compute_disorder`
  - Implementar a métrica de desordem definida no PDF.
  - Garantir resultado entre `0` e `1`.
- `sort_5`
  - Enviar os menores elementos para B.
  - Ordenar os restantes.
  - Recuperar os elementos.

### Trabalho conjunto

Definir a interface dos algoritmos:

```c
void	simple_sort(t_stack **a, t_stack **b);
void	medium_sort(t_stack **a, t_stack **b);
void	complex_sort(t_stack **a, t_stack **b);
void	adaptive_sort(t_stack **a, t_stack **b);
```

### Meta

Ao final do dia, a infraestrutura necessária para qualquer estratégia de ordenação deve estar pronta.

---

## 🟥 Dia 4: CLI + Simple Sort

*Implementar a primeira estratégia completa e integrar as opções do programa.*

### Guilherme — Simple Sort

- `simple_sort`
- Implementar a estratégia O(n²) definida pelo PDF.
- Integrar os casos pequenos:
  - `sort_2`
  - `sort_3`
  - `sort_5`

### Jhony — CLI e Estratégia

- `parse_flags`
- Interpretar:
  - `--simple`
  - `--medium`
  - `--complex`
  - `--adaptive`
  - `--bench`
- Criar a estrutura necessária para representar a estratégia escolhida.
- Definir a estratégia padrão.

### Trabalho conjunto

Integrar tudo no `main`:

```text
argv
 ↓
parse_flags
 ↓
init_stack
 ↓
assign_index
 ↓
seleção da estratégia
 ↓
sort
```

### Meta

Conseguir executar:

```bash
./push_swap --simple 3 2 1
```

e receber uma sequência válida de operações.

---

## 🟪 Dia 5: Medium Sort

*Implementar e otimizar a estratégia intermediária.*

### Guilherme

- Implementar a lógica principal do `medium_sort`.
- Definir a divisão dos elementos em blocos/faixas.
- Implementar o envio dos elementos de A para B.
- Controlar a estratégia de seleção dos elementos.

### Jhony

- Implementar a estratégia de recuperação de B para A.
- Otimizar rotações:
  - `rb`
  - `rrb`
  - eventualmente `rr`
- Trabalhar na redução do número de operações.
- Criar testes específicos para diferentes níveis de disorder.

### Trabalho conjunto

Comparar os resultados:

```text
100 números
500 números
```

E ajustar o tamanho dos blocos.

### Meta

Ter uma primeira versão funcional e mensurável do `medium_sort`.

---

## 🟦 Dia 6: Complex Sort + Adaptive

*Implementar a estratégia mais eficiente e conectar todas as estratégias.*

### Guilherme — Complex Sort

- Implementar `complex_sort`.
- Primeira abordagem: Radix Sort baseado nos índices.
- Utilizar operações bitwise:
  - `>>`
  - `&`
- Fluxo:
  - bit `0` → `pb`
  - bit `1` → `ra`
  - depois `pa`
- Medir quantidade de operações.

### Jhony — Adaptive Sort

- Implementar `adaptive_sort`.
- Calcular `disorder`.
- Selecionar automaticamente a estratégia:
  - baixa desordem → `simple_sort`
  - desordem intermediária → `medium_sort`
  - alta desordem → `complex_sort`
- Utilizar os thresholds definidos pelo PDF.

### Trabalho conjunto

- Comparar `simple`, `medium` e `complex`.
- Validar se o `adaptive` realmente escolhe a estratégia adequada.
- Ajustar thresholds com dados reais.

### Meta

As quatro estratégias devem funcionar:

```text
simple
medium
complex
adaptive
```

---

## ⬜ Dia 7: Benchmark, Testes e Entrega

*Integrar, medir, corrigir e preparar a entrega.*

### Guilherme — Benchmark e Performance

Implementar o modo:

```bash
./push_swap --bench ...
```

Exibir no `stderr`:

- Disorder.
- Estratégia escolhida.
- Complexidade.
- Total de operações.
- Contagem de cada movimento.

Também:

- Comparar performance das quatro estratégias.
- Verificar quantidade de operações.
- Identificar gargalos.

### Jhony — Qualidade e Testes

Executar e corrigir:

- Testes de parsing.
- Números duplicados.
- Overflow/underflow.
- Entrada vazia.
- Um único número.
- Entrada já ordenada.
- Entrada invertida.
- Números negativos.
- `INT_MIN`.
- `INT_MAX`.
- Entradas grandes.
- Todas as estratégias.

Também:

- Valgrind.
- Memory leaks.
- Norminette.

### Trabalho conjunto — README e Entrega

- Escrever o `README.md`.
- Documentar:
  - arquitetura;
  - parsing;
  - `assign_index`;
  - disorder;
  - `simple`;
  - `medium`;
  - `complex`;
  - `adaptive`;
  - complexidades;
  - benchmark;
  - decisões de projeto.
- Baixar e executar testers.
- Corrigir problemas encontrados.
- Fazer revisão final do código.

---

# ⚖️ Distribuição de Esforço

| Dia | Guilherme | Jhony |
|---|---|---|
| Dia 1 | 🟰 Fundação | 🟰 Fundação |
| Dia 2 | 🟰 Swap/Push | 🟰 Rotate |
| Dia 3 | `assign_index` + casos pequenos | `disorder` + casos pequenos |
| Dia 4 | `simple_sort` | CLI + flags |
| Dia 5 | Parte principal do `medium` | Parte principal do `medium` |
| Dia 6 | `complex_sort` | `adaptive_sort` |
| Dia 7 | Benchmark + performance | Testes + qualidade |
| **Carga** | **≈ 50%** | **≈ 50%** |

## Princípio da divisão

A divisão não é baseada em **quantidade de funções**, mas em **complexidade e tempo de implementação**.

- `complex_sort` é mais pesado → Guilherme fica praticamente focado nele.
- `medium_sort` é dividido entre os dois.
- `adaptive_sort` é acompanhado pelo Jhony porque depende diretamente do `disorder` que ele implementou.
- CLI e benchmark são trabalhos de infraestrutura, compensando as partes algorítmicas.
- Testes e qualidade recebem um dia dedicado, em vez de serem tratados como "tarefas menores".
- Todas as integrações importantes são feitas **em conjunto**, evitando que uma pessoa fique responsável sozinha pela arquitetura final.
