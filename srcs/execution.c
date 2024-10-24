/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 20:53:47 by dongjle2          #+#    #+#             */
/*   Updated: 2024/10/24 17:50:08 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	call_exec(t_exec *exec_cmd, t_list *env_list, t_parse_data parse,
		char *file)
{
	extern char	**environ;

	environ = mk_env_list(env_list);
	execve(file, exec_cmd->arg, environ);
	free(file);
	free_parse(parse);
	ft_lstclear(&env_list, free);
	exit(1);
}

char	*get_valid_exec(t_exec *exec_cmd, t_list *env_list)
{
	int		path_exist;
	char	*full_path;
	int		ac;

	if (ft_strcmp(exec_cmd->arg[0], "") == 0)
		return (NULL);
	ac = access(exec_cmd->arg[0], X_OK);
	if (ac == 0)
		return (ft_strdup(exec_cmd->arg[0]));
	path_exist = find_env(env_list, "PATH");
	if (path_exist == 1)
	{
		full_path = get_full_path(exec_cmd, env_list);
		return (full_path);
	}
	else if (path_exist == 0)
		return (NULL);
	return (NULL);
}

void	run_exec(t_exec *exec_cmd, t_param *param, t_parse_data parse)
{
	pid_t	pid;
	int		status;
	char	*file;

	file = get_valid_exec(exec_cmd, param->env_list);
	if (file == NULL)
	{
		printf("command not found\n");
		param->special.question_mark = 127;
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		call_exec(exec_cmd, param->env_list, parse, file);
	}
	else
	{
		free(file);
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		param->special.question_mark = WEXITSTATUS(status);
		signal(SIGINT, signal_handler);
	}
	return ;
}

int	set_exec(t_exec *exec_cmd, t_param *param, t_parse_data parse)
{
	int	builtin_ret;

	if (ck_builtin(exec_cmd->arg[0]) == 1)
	{
		builtin_ret = call_builtin(exec_cmd, param, parse);
		param->special.question_mark = builtin_ret;
	}
	else
	{
		run_exec(exec_cmd, param, parse);
	}
	return (0);
}
