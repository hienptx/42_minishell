#include "../includes/minishell.h"

void	set_redir(t_redir *redir_cmd, t_param *param, t_parse_data parse)
{
	t_redir	*predir_cmd;
	int		saved_stdin;
	int		saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	predir_cmd = redir_cmd;
	while (predir_cmd)
	{
		dup_fd(predir_cmd);
		close(predir_cmd->fd);
		predir_cmd = predir_cmd->next;
	}
	iterate_ast((t_cmd *)redir_cmd->cmd, param, parse);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

// void	dup_fd(t_redir *redir_cmd)
// {
// 	if ((redir_cmd->fd == 0) || (redir_cmd->fd == 1) || (redir_cmd->fd == 2))
// 	{
// 		get_file_fd(redir_cmd);
// 		if (redir_cmd->fd == 0)
// 			dup2(redir_cmd->fd, STDIN_FILENO);
// 		else
// 			dup2(redir_cmd->fd, STDOUT_FILENO);
// 	}
// 	else
// 		dup2(redir_cmd->fd, STDIN_FILENO);
// }

void	dup_fd(t_redir *redir_cmd)
{
	int fd;

	if (redir_cmd->fd == 0)
	{
		fd = get_file_fd(redir_cmd);
		if (fd == -1)
			return ;
		dup2(fd, STDIN_FILENO);
	}
	else if ((redir_cmd->fd == 1) || (redir_cmd->fd == 2))
	{
		fd = get_file_fd(redir_cmd);
		if (fd == -1)
			return ;
		dup2(fd, STDOUT_FILENO);
	}
	else
		dup2(redir_cmd->fd, STDIN_FILENO);
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
		panic_sms("open", 1);
	return(redir_cmd->fd);
}
