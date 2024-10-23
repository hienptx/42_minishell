/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 20:53:47 by dongjle2          #+#    #+#             */
/*   Updated: 2024/10/23 21:41:41 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/stat.h>

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
		(void)status;
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

int	is_executable(const char *path)
{
	struct stat	buffer;

	return (stat(path, &buffer) == 0 && buffer.st_mode & S_IXUSR);
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

char	**get_all_path(char *env_path)
{
	if (env_path == NULL)
		return (NULL);
	env_path += 5;
	return (ft_split(env_path, ':'));
}

char	*get_executable_path(char *env_path, char *prog_name)
{
	char	**cur_dir;
	char	*executable_path;
	size_t	i;

	i = 0;
	cur_dir = get_all_path(env_path);
	if (cur_dir == NULL)
		panic_sms("malloc fails", 1);
	free(env_path);
	while (cur_dir[i])
	{
		executable_path = ft_strsjoin(NULL, cur_dir[i], "/", prog_name, NULL);
		if (executable_path == NULL)
			panic_sms("malloc fails", 1);
		if (access(executable_path, X_OK) == 0)
		{
			ft_free(cur_dir);
			return (executable_path);
		}
		i++;
		free(executable_path);
		executable_path = NULL;
	}
	ft_free(cur_dir);
	return (NULL);
}
