/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   medium_chunks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilherme <guilherme@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 22:00:00 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/17 22:00:00 by guilherme        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	medium_chunk_size(int size)
{
	int	root;

	if (size <= 0)
		return (0);
	root = 1;
	while (root < size / root)
		root++;
	return (root);
}

static void	push_chunk_value(t_stack **a, t_stack **b, int lower, int size)
{
	pb(a, b);
	if ((*b)->index < lower + size / 2)
		rb(b);
}

static void	update_chunk(int *lower, int *limit, int *pushed, int size)
{
	(*pushed)++;
	if (*pushed == size)
	{
		*lower = *limit;
		*limit += size;
		*pushed = 0;
	}
}

void	medium_push_chunks(t_stack **a, t_stack **b, int chunk_size)
{
	int	limit;
	int	lower;
	int	pushed;

	if (!a || !*a || !b || chunk_size <= 0)
		return ;
	limit = chunk_size;
	lower = 0;
	pushed = 0;
	while (*a)
	{
		if ((*a)->index < limit)
		{
			push_chunk_value(a, b, lower, chunk_size);
			update_chunk(&lower, &limit, &pushed, chunk_size);
		}
		else
			ra(a);
	}
}
