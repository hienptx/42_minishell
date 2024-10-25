/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qsort.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 21:37:01 by dongjle2          #+#    #+#             */
/*   Updated: 2024/10/25 21:05:57 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"
#include "../../includes/minishell.h"

static void	swap(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	partition(char **arr, int low, int high)
{
	char	*pivot;
	int		i;
	int		j;

	pivot = arr[high];
	i = low - 1;
	j = low;
	while (j < high)
	{
		if (ft_strcmp(arr[j], pivot) < 0)
		{
			i++;
			swap(&arr[i], &arr[j]);
		}
		j++;
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

void	quick_sort(char **arr, int low, int high)
{
	int	pi;

	if (low < high)
	{
		pi = partition(arr, low, high);
		quick_sort(arr, low, pi - 1);
		quick_sort(arr, pi + 1, high);
	}
}

void	get_exit_param(t_param *param, t_parse_data parse, char *args1,
		char *args2)
{
	int		is_nummeric;
	char	*ptr;

	ptr = args1;
	if (args2 != NULL)
		ft_exit(" too many arguments\n", 1, param, parse);
	else
	{
		is_nummeric = 1;
		if (*args1 == '+' || *args1 == '-')
			args1++;
		if (*args1 == '\0')
			ft_exit(" numeric argument required\n", 255, param, parse);
		while (*args1 != '\0')
		{
			if (!ft_isdigit(*args1))
				is_nummeric = 0;
			args1++;
		}
		if (!is_nummeric)
			ft_exit(" numeric argument required\n", 255, param, parse);
		else
			ft_exit("\0", ft_atoi(ptr), param, parse);
	}
}

int	export_syntax_valid(char *s)
{
	while (*s != '\0')
	{
		if (!ft_isalnum(*s) && *s != '_' && *s != '=')
			return (0);
		s++;
	}
	return (1);
}
