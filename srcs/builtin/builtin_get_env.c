/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_get_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 22:09:54 by dongjle2          #+#    #+#             */
/*   Updated: 2024/10/19 22:16:19 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"
#include "../../includes/minishell.h"

char	*get_env_key(char *env)
{
	char	*delim;

	delim = ft_strchr(env, '=');
	if (delim == NULL)
		return (ft_strdup(env));
	return (get_key_or_value("key", env, delim - env));
}

char	*get_env_value(char *env_key, t_list *env_list)
{
	char	*env;
	char	*delim;

	env = get_env(env_key, env_list);
	if (env == NULL)
		return (NULL);
	delim = ft_strchr(env, '=');
	if (delim == NULL)
		return (NULL);
	return (get_key_or_value("value", env, delim - env));
}

char	*get_key_or_value(char *key_or_val, char *env, size_t key_len)
{
	size_t	val_len;
	char	*key;
	char	*val;

	if (*key_or_val == 'k')
	{
		key = ft_substr(env, 0, key_len);
		if (key == NULL)
			panic_sms("malloc", 1);
		return (key);
	}
	else
	{
		val_len = ft_strlen(env) - key_len - 1;
		val = ft_substr(env, key_len + 1, val_len);
		if (val == NULL)
			panic_sms("malloc", 1);
		return (val);
	}
}

char	*get_env(char *env_key, t_list *env_list)
{
	size_t	key_len;
	char	*cur_env;

	key_len = ft_strlen(env_key);
	while (env_list)
	{
		cur_env = (char *)env_list->content;
		if (ft_strncmp(env_key, cur_env, key_len) == 0
			&& (cur_env[key_len] == '=' || cur_env[key_len] == '\0'))
		{
			return (cur_env);
		}
		env_list = env_list->next;
	}
	return (NULL);
}

int	find_env(t_list *env_list, char *key)
{
	char	*cur_key;

	while (env_list)
	{
		cur_key = get_env_key((char *)env_list->content);
		if (cur_key == NULL)
		{
			perror("malloc");
			return (-1);
		}
		if (ft_strcmp(cur_key, key) == 0)
		{
			free(cur_key);
			return (1);
		}
		env_list = env_list->next;
		free(cur_key);
	}
	return (0);
}
