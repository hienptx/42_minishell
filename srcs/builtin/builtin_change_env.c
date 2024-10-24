/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_change_env.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 22:08:52 by dongjle2          #+#    #+#             */
/*   Updated: 2024/10/24 19:28:46 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"
#include "../../includes/minishell.h"

int	add_env(t_list **env_list, char *new_env)
{
	t_list	*new_node;

	new_node = ft_lstnew(new_env);
	if (new_node == NULL)
		return (1);
	ft_lstadd_back(env_list, new_node);
	return (0);
}

void	mod_env(t_list *env_list, const char *new_env)
{
	free(env_list->content);
	env_list->content = NULL;
	env_list->content = (char *)new_env;
	return ;
}

void	rm_node(t_list **env_list, t_list **prev, t_list *cur)
{
	if (cur->content)
		free(cur->content);
	if (*prev)
		(*prev)->next = (cur)->next;
	else
		*env_list = cur->next;
	free(cur);
	cur = NULL;
}

t_list	*find_env_node(t_list *env_list, const char *key_to_remove,
		t_list **prev)
{
	t_list	*cur;
	char	*cur_key;

	cur = env_list;
	*prev = NULL;
	while (cur)
	{
		cur_key = get_env_key((char *)cur->content);
		if (cur_key == NULL)
			panic_sms("malloc", 1);
		if (ft_strcmp(cur_key, key_to_remove) == 0)
		{
			free(cur_key);
			return (cur);
		}
		free(cur_key);
		*prev = cur;
		cur = cur->next;
	}
	return (NULL);
}

int	rm_env(t_list **env_list, const char *key_to_remove)
{
	t_list	*prev;
	t_list	*cur;

	cur = find_env_node(*env_list, key_to_remove, &prev);
	if (cur)
	{
		rm_node(env_list, &prev, cur);
		return (0);
	}
	return (0);
}
