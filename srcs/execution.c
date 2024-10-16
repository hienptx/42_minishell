#include "../includes/minishell.h"

int	call_exec(t_exec *exec_cmd, t_list *env_list, t_parse_data parse)
{
	extern char	**environ;
	char		*path;
	int			path_exist;

	if (access(exec_cmd->arg[0], X_OK) == 0)
	{
		environ = mk_env_list(env_list);
		execve(exec_cmd->arg[0], exec_cmd->arg, environ);
		exit(1);
	}
	path_exist = find_env(env_list, "PATH");
	if (path_exist == 1)
	{
		path = get_executable_path(get_env_value("PATH", env_list),
				exec_cmd->arg[0]);
		if (path == NULL)
		{
			printf("%s: %s\n", exec_cmd->arg[0], "command not found");
			free_parse(parse);
			ft_lstclear(&env_list, free);
			exit(127);
		}
		exec_cmd->arg[0] = path;
		environ = mk_env_list(env_list);
		execve(path, exec_cmd->arg, environ);
		perror(exec_cmd->arg[0]);
		exit(1);
	}
	else if (path_exist == 0)
	{
		return (0);
	}
	else
		return (-1);
}

void	run_exec(t_exec *exec_cmd, t_param *param, t_parse_data parse)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		call_exec(exec_cmd, param->env_list, parse);
		(void)status;
	}
	else
	{
		waitpid(pid, &status, 0);
		// update_env(env_list, "?", ft_strjoin("?=",
		// ft_itoa(WEXITSTATUS(status));
		param->special.question_mark = WEXITSTATUS(status);
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
		run_exec(exec_cmd, param, parse);
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
	char	*ret;
	size_t	i;

	i = 0;
	cur_dir = get_all_path(env_path);
	free(env_path);
	if (cur_dir == NULL)
		panic_sms("malloc fails", 1);
	while (cur_dir[i])
	{
		executable_path = ft_strsjoin(NULL, cur_dir[i], "/", prog_name, NULL);
		if (executable_path == NULL)
			panic_sms("malloc fails", 1);
		if (access(executable_path, X_OK) == 0)
		{
			ret = ft_strdup(executable_path);
			ft_free(cur_dir);
			free(executable_path);
			return (ret);
		}
		i++;
		free(executable_path);
		executable_path = NULL;
	}
	ft_free(cur_dir);
	return (NULL);
}
