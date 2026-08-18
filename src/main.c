/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilherme <guilherme@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 00:18:03 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/13 00:45:24 by guilherme        ###   ########.fr       */
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

	if (argc < 2)
		return (0);
	if (!validate_numbers(argc, argv, 1))
		return (print_error());
	stack_a = init_stack(argc, argv);
	if (!stack_a || !ft_check_duplicates(stack_a))
	{
		ft_freestack(&stack_a);
		return (print_error());
	}
	ft_print_stack(stack_a);
	ft_freestack(&stack_a);
	return (0);
}
