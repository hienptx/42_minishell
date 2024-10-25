/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:28:49 by dongjle2          #+#    #+#             */
/*   Updated: 2024/10/25 21:07:01 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"
#include "../../includes/minishell.h"

int	cd(t_list *env_list, char *x[])
{
	const char	*path;
	char		*oldpwd;

	path = x[1];
	if (path == NULL)
		path = getenv("HOME");
	if (path)
	{
		oldpwd = getcwd(NULL, 0);
		if (oldpwd == NULL)
			panic_sms("getcwd failed", 1);
		if (change_dir(env_list, path, oldpwd) != 0)
		{
			panic_sms("malloc", 0);
			free(oldpwd);
			return (1);
		}
		free(oldpwd);
	}
	else
	{
		perror("minishell: cd: HOME is not set");
		return (1);
	}
	return (0);
}

int	echo_n_option(char **x, int start_idx)
{
	size_t	i;

	i = start_idx + 1;
	if (x[1] == NULL)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	while (x[i])
	{
		write(STDOUT_FILENO, x[i], ft_strlen(x[i]));
		if (x[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (start_idx == 0)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

int	echo(char **x)
{
	if (x[1] != NULL && ft_strcmp("-n", x[1]) == 0)
		echo_n_option(x, 1);
	else
		echo_n_option(x, 0);
	return (0);
}

int	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (2);
	write(1, cwd, strlen(cwd));
	write(1, "\n", 1);
	free(cwd);
	cwd = NULL;
	return (0);
}

int	export(t_list *env_list, char *x[])
{
	size_t	i;
	char	*key;
	char	*tmp;

	i = 1;
	if (x[i] == NULL)
		return (display_sorted_env(env_list), 0);
	while (x[i])
	{
		if (!export_syntax_valid(x[1]))
			return (printf("%s': not a valid identifier\n", x[i]), 1);
		key = get_env_key(x[i]);
		if (key == NULL)
			return (1);
		tmp = ft_strdup(x[i++]);
		if (update_env(env_list, key, tmp) == 1)
			return (free(tmp), 1);
		free(key);
		key = NULL;
	}
	return (0);
}
