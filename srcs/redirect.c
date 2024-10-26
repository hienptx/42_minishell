/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:29:01 by dongjle2          #+#    #+#             */
/*   Updated: 2024/10/26 02:42:13 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>
#include <unistd.h>

int	apply_redir(t_redir *redir_cmd, t_param *param)
{
	t_redir	*predir_cmd;

	predir_cmd = redir_cmd;
	while (predir_cmd)
	{
		if (dup_fd(predir_cmd) == -1)
		{
			param->special.question_mark = 1;
			return (-1);
		}
		close(predir_cmd->fd);
		predir_cmd = predir_cmd->next;
	}
	return (0);
}

void	set_redir(t_redir *redir_cmd, t_param *param, t_parse_data parse)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		perror("dup");
		return ;
	}
	if (apply_redir(redir_cmd, param) == -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return ;
	}
	iterate_ast((t_cmd *)redir_cmd->cmd, param, parse);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

int	dup_fd(t_redir *redir_cmd)
{
	int	fd;

	if (redir_cmd->fd == 0)
	{
		fd = get_file_fd(redir_cmd);
		if (fd == -1)
			return (-1);
		dup2(fd, STDIN_FILENO);
	}
	else if ((redir_cmd->fd == 1) || (redir_cmd->fd == 2))
	{
		fd = get_file_fd(redir_cmd);
		if (fd == -1)
			return (-1);
		dup2(fd, STDOUT_FILENO);
	}
	else
		dup2(redir_cmd->fd, STDIN_FILENO);
	return (0);
}

int	get_file_fd(t_redir *redir_cmd)
{
	int	oflag;

	oflag = 0;
	if (redir_cmd->fd == 0)
		oflag = O_RDONLY;
	else if (redir_cmd->fd == 1)
		oflag = (O_WRONLY | O_CREAT | O_TRUNC);
	else if (redir_cmd->fd == 2)
		oflag = (O_WRONLY | O_CREAT | O_APPEND);
	redir_cmd->fd = open(redir_cmd->file_name, oflag, 0644);
	if (redir_cmd->fd == -1)
	{
		perror(redir_cmd->file_name);
		return (-1);
	}
	return (redir_cmd->fd);
}
