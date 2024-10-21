/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_sort_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 23:00:15 by dongjle2          #+#    #+#             */
/*   Updated: 2024/10/21 23:02:23 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"
#include "../../includes/minishell.h"

void	display_sorted_env(t_list *env_list)
{
	char	**env_arr;
	size_t	arr_len;
	size_t	i;

	i = 0;
	env_arr = mk_env_list_cp(env_list);
	arr_len = split_len(env_arr);
	quick_sort(env_arr, 0, arr_len - 1);
	while (i < arr_len)
	{
		printf("%s\n", env_arr[i++]);
	}
	ft_free(env_arr);
}


char	**mk_env_list_cp(t_list *env_list)
{
	t_list	*cur;
	size_t	len_env_list;
	char	**env_arr;
	size_t	i;

	i = 0;
	cur = env_list;
	len_env_list = ft_lstsize(env_list);
	env_arr = (char **)malloc((len_env_list + 1) * sizeof(char *));
	if (!env_arr)
		return (NULL);
	while (cur != NULL)
	{
		env_arr[i] = ft_strdup((char *)cur->content);
		if (!env_arr[i])
		{
			free_pre_allocated(env_arr, i);
			return (NULL);
		}
		cur = cur->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

void	free_pre_allocated(char **env_arr, size_t i)
{
	while (i > 0)
	{
		i--;
		free(env_arr[i]);
	}
	free(env_arr);
}

size_t	split_len(char **split)
{
	size_t	i;

	i = 0;
	while (split[i])
		i++;
	return (i);
}
