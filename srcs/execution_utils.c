/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 20:53:36 by dongjle2          #+#    #+#             */
/*   Updated: 2024/10/22 20:59:33 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_w_path_env(t_exec *exec_cmd, t_list *env_list, t_parse_data parse)
{
	extern char	**environ;
	char		*path;

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
	free(environ);
	free(path);
	exit(1);
}
