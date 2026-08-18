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

void	ft_print_stack(t_stack *stack)
{
	while (stack)
	{
		ft_putnbr_fd(stack->value, 1);
		ft_putchar_fd('\n', 1);
		stack = stack->next;
	}
}

static int	print_error(void)
{
	ft_putstr_fd("Error\n", 2);
	return (1);
}

int	main(int argc, char **argv)
{
	t_stack	*stack_a;
	t_input	input;

	if (argc < 2)
		return (0);
	if (!normalize_args(argc - 1, argv + 1, &input))
		return (print_error());
	if (!validate_numbers(input.count, input.values, 0))
	{
		free_input(&input);
		return (print_error());
	}
	stack_a = init_stack(input.count, input.values);
	free_input(&input);
	if (!stack_a || !ft_check_duplicates(stack_a))
	{
		ft_freestack(&stack_a);
		return (print_error());
	}
	ft_print_stack(stack_a);
	ft_freestack(&stack_a);
	return (0);
}
