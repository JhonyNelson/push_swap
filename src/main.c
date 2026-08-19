/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 00:18:03 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/18 15:56:09 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	print_error(void)
{
	ft_putstr_fd("Error\n", 2);
	return (1);
}

int	main(int argc, char **argv)
{
	t_stack	*stack_a;
	t_stack	*stack_b;
	int		strategy;
	int		bench;
	int		first;

	if (argc < 2)
		return (0);
	first = parse_flags(argc, argv, &strategy, &bench);
	if (first < 0 || first == argc || bench)
		return (print_error());
	stack_a = init_stack_from_args(argc, argv, first);
	if (!stack_a)
		return (print_error());
	stack_b = NULL;
	if (!execute_strategy(&stack_a, &stack_b, strategy))
	{
		ft_freestack(&stack_a);
		ft_freestack(&stack_b);
		return (print_error());
	}
	ft_freestack(&stack_a);
	ft_freestack(&stack_b);
	return (0);
}
