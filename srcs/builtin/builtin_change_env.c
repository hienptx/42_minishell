/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_change_env.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 22:08:52 by dongjle2          #+#    #+#             */
/*   Updated: 2024/10/23 21:40:45 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"
#include "../../includes/minishell.h"

int	update_env(t_list *env_list, const char *key, char *new_env)
{
	char	*cur_key;
	t_list	*node;

	node = env_list;
	while (env_list)
	{
		cur_key = get_env_key((char *)env_list->content);
		if (cur_key == NULL)
			return (1);
		if (ft_strcmp(cur_key, key) == 0)
		{
			mod_env(env_list, new_env);
			free(cur_key);
			cur_key = NULL;
			return (0);
		}
		free(cur_key);
		cur_key = NULL;
		env_list = env_list->next;
	}
	if (add_env(&node, new_env) == 1)
		return (1);
	return (0);
}

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

// void	rm_node(t_list **env_list, t_list **prev, t_list *cur)
// {
// 	if (*prev)
// 		(*prev)->next = (cur)->next;
// 	else
// 		*env_list = cur->next;
// 	free(cur);
// 	(cur) = NULL;
// }

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

int	rm_env(t_list **env_list, const char *key_to_remove)
{
	t_list	*cur;
	t_list	*prev;
	char	*cur_key;

	cur = *env_list;
	prev = NULL;
	while (cur)
	{
		cur_key = get_env_key((char *)cur->content);
		if (cur_key == NULL)
			panic_sms("malloc", 1);
		if (ft_strcmp(cur_key, key_to_remove) == 0)
		{
			rm_node(env_list, &prev, cur);
			free(cur_key);
			cur_key = NULL;
			return (0);
		}
		if (cur_key != NULL)
		{
			free(cur_key);
			cur_key = NULL;
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}
