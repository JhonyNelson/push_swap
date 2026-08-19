/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strategy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 17:00:00 by guilamar          #+#    #+#             */
/*   Updated: 2026/08/18 17:00:00 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	execute_strategy(t_stack **a, t_stack **b, int strategy)
{
	if (!a || !*a || !b)
		return (0);
	if (ft_stackis_sorted(*a))
		return (1);
	assign_index(a);
	if (strategy == STRATEGY_SIMPLE)
		simple_sort(a, b);
	else if (strategy == STRATEGY_MEDIUM)
		medium_sort(a, b);
	else if (strategy == STRATEGY_COMPLEX)
		complex_sort(a, b);
	else
		return (0);
	return (1);
}
