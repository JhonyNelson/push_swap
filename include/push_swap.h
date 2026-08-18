/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilherme <guilherme@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 21:58:46 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/14 11:04:29 by guilherme        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft.h"
# include <stdlib.h>
# include <unistd.h>

typedef struct s_stack
{
	int				value;
	int				index;
	struct s_stack	*prev;
	struct s_stack	*next;
}					t_stack;

/* ================= FUNÇÕES BASE ================= */

t_stack				*ft_new_node(int value);
t_stack				*ft_stacklast(t_stack *stack);
void				ft_stackadd_back(t_stack **stack, t_stack *new_node);
void				ft_stackadd_front(t_stack **stack, t_stack *new_node);
int					ft_stacksize(t_stack *stack);
void				ft_freestack(t_stack **stack);

/* ================ PARSING & ERROS ================ */
int					ft_isdigit_str(char *c);
int					ft_check_args(int argc, char **argv);
int					ft_check_duplicates(t_stack *stack);
t_stack				*init_stack(int argc, char **argv);

/* ================ MOVIMENTOS (OPERATIONS) ================ */
/* Swap */
void				ft_swap(t_stack **stack);
void				sa(t_stack **stack_a);
void				sb(t_stack **stack_b);
void				ss(t_stack **stack_a, t_stack **stack_b);

/* Push */
void				ft_push(t_stack **src, t_stack **dest);
void				pa(t_stack **stack_a, t_stack **stack_b);
void				pb(t_stack **stack_a, t_stack **stack_b);

/* Rotate */
void				ft_rotate(t_stack **stack);
void				ra(t_stack **stack_a);
void				rb(t_stack **stack_b);
void				rr(t_stack **stack_a, t_stack **stack_b);

/* Reverse Rotate */
void				ft_rev_rotate(t_stack **stack);
void				rra(t_stack **stack_a);
void				rrb(t_stack **stack_b);
void				rrr(t_stack **stack_a, t_stack **stack_b);

/* ================ MÉTRICAS E ÍNDICES ================ */
double				compute_disorder(t_stack *stack_a);
void				assign_index(t_stack **stack_a);

/* ============ SELEÇÃO DE ESTRATÉGIAS (FLAGS) ============ */
void				parse_flags(int argc, char **argv, int *strategy,
						int *bench);

/* ================ ALGORITMOS DE ORDENAÇÃO ================ */
/* 1. Simples O(n^2) */
void				simple_sort(t_stack **a, t_stack **b);

/* 2. Médio O(n*sqrt(n)) */
void				medium_sort(t_stack **a, t_stack **b);
int					medium_block_size(int size);
void				medium_push_blocks(t_stack **a, t_stack **b,
						int block_size);
void				medium_restore(t_stack **a, t_stack **b);

/* 3. Complexo O(n log n) - Ex: Radix */
void				complex_sort(t_stack **a, t_stack **b);

/* 4. Adaptativo */
void				adaptive_sort(t_stack **a, t_stack **b, double disorder);

/* 5. Ordenação Pequena (Auxiliares) */
void				sort_3(t_stack **a);
void				sort_5(t_stack **a, t_stack **b);

#endif
