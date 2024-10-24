/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 20:53:36 by dongjle2          #+#    #+#             */
/*   Updated: 2024/10/24 17:44:01 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_full_path(t_exec *exec_cmd, t_list *env_list)
{
	char	*full_path;

	full_path = get_executable_path(get_env_value("PATH", env_list),
			exec_cmd->arg[0]);
	return (full_path);
}

int	is_executable(const char *path)
{
	struct stat	buffer;

	return (stat(path, &buffer) == 0 && buffer.st_mode & S_IXUSR);
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
