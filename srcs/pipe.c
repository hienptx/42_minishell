/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 22:19:02 by dongjle2          #+#    #+#             */
/*   Updated: 2024/10/26 02:00:33 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	left_pipe(t_pipe *pipe_cmd, int *fd, t_param *param, t_parse_data parse)
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	iterate_ast((t_cmd *)pipe_cmd->left, param, parse);
	free_parse(parse);
	ft_lstclear(&param->env_list, free);
	exit(1);
}

void	right_pipe(t_pipe *pipe_cmd, int *fd, t_param *param,
		t_parse_data parse)
{
	int	status;

	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	iterate_ast((t_cmd *)pipe_cmd->right, param, parse);
	status = param->special.question_mark;
	free_parse(parse);
	ft_lstclear(&param->env_list, free);
	exit(status);
}

int	set_pipe(t_pipe *pipe_cmd, t_param *param, t_parse_data parse)
{
	int		fd[2];
	pid_t	pid_l;
	pid_t	pid_r;
	int		status;

	if (pipe(fd) == -1)
		return (-1);
	pid_l = fork();
	if (pid_l == 0)
	{
		left_pipe(pipe_cmd, fd, param, parse);
	}
	pid_r = fork();
	if (pid_r == 0)
	{
		right_pipe(pipe_cmd, fd, param, parse);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid_l, NULL, 0);
	waitpid(pid_r, &status, 0);
	param->special.question_mark = WEXITSTATUS(status);
	return (0);
}
