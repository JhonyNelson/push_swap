#include "push_swap.h"

static int	max_position(t_stack *stack)
{
	int	max;
	int	position;
	int	index;

	max = stack->index;
	position = 0;
	index = 0;
	while (stack)
	{
		if (stack->index > max)
		{
			max = stack->index;
			position = index;
		}
		index++;
		stack = stack->next;
	}
	return (position);
}

static void	move_max_to_top(t_stack **stack)
{
	int	position;
	int	size;

	position = max_position(*stack);
	size = ft_stacksize(*stack);
	if (position <= size / 2)
		while (position-- > 0)
			rb(stack);
	else
	{
		position = size - position;
		while (position-- > 0)
			rrb(stack);
	}
}

void	medium_restore(t_stack **a, t_stack **b)
{
	if (!a || !b)
		return ;
	while (*b)
	{
		move_max_to_top(b);
		pa(a, b);
	}
}
