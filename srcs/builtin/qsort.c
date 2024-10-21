/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qsort.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 21:37:01 by dongjle2          #+#    #+#             */
/*   Updated: 2024/10/21 23:02:20 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"
#include "../../includes/minishell.h"

static void	swap(char **a, char **b)
{
	char *temp = *a;
	*a = *b;
	*b = temp;
}

int	partition(char **arr, int low, int high)
{
	char *pivot = arr[high];
	int i = low - 1;
	int j = low;

	while (j < high) {
		if (ft_strcmp(arr[j], pivot) < 0) {
			i++;
			swap(&arr[i], &arr[j]);
		}
		j++;
	}
	swap(&arr[i + 1], &arr[high]);
	return i + 1;
}

void quick_sort(char **arr, int low, int high)
{
	if (low < high)
	{
		int pi = partition(arr, low, high);
		quick_sort(arr, low, pi - 1);
		quick_sort(arr, pi + 1, high);
	}
}
