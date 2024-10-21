/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 20:53:36 by dongjle2          #+#    #+#             */
/*   Updated: 2024/10/21 18:40:47 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_w_path_env(t_exec *exec_cmd, t_list *env_list, t_parse_data parse)
{
	extern char	**environ;
	char		*path;

	path = get_executable_path(get_env_value("PATH", env_list), \
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
	// ft_lstclear(&env_list, free);
	free(path);
	// free_parse(parse);
	exit(1);
}
