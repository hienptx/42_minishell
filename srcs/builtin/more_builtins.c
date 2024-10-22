/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:21:13 by hipham            #+#    #+#             */
/*   Updated: 2024/10/22 13:54:33 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"
#include "../../includes/minishell.h"

int	ck_builtin(char *executable_name)
{
	size_t		i;
	const char	*builtins[8] = {"echo", "cd", "pwd", "export", "unset", "env",
		"exit", NULL};

	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(builtins[i], executable_name) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	call_builtin(t_exec *exec_cmd, t_param *param, t_parse_data parse)
{
	char	**args;
	int		ret;

	args = exec_cmd->arg;
	if (strcmp(args[0], "echo") == 0)
		ret = echo(args);
	else if (ft_strcmp(args[0], "cd") == 0)
		ret = cd(param->env_list, args);
	else if (ft_strcmp(args[0], "pwd") == 0)
		ret = pwd();
	else if (ft_strcmp(args[0], "export") == 0)
		ret = export(param->env_list, args);
	else if (ft_strcmp(args[0], "unset") == 0)
		ret = unset(args, param->env_list);
	else if (ft_strcmp(args[0], "env") == 0)
		ret = env(param->env_list);
	else if (ft_strcmp(args[0], "exit") == 0)
	{
		ft_lstclear(&param->env_list, free);
		free_parse(parse);
		exit(0);
	}
	else
		ret = 0;
	return (ret);
}

int	unset(char **args, t_list *env_list)
{
	while (*++args)
	{
		rm_env(&env_list, *args);
	}
	return (0);
}

void	set_env(t_list **env_list)
{
	*env_list = cp_env_list();
	return ;
}

int	cd_external(const char *path, char **cwd)
{
	if (chdir(path) == -1)
	{
		perror("chdir");
		return (-1);
	}
	*cwd = getcwd(NULL, 0);
	if (*cwd == NULL)
		return (2);
	return (0);
}
