#include "../includes/minishell.h"

void	left_pipe(t_pipe *pipe_cmd, int *fd, t_param *param)
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	iterate_ast((t_cmd *)pipe_cmd->left, param);
	exit(0);
}

void	right_pipe(t_pipe *pipe_cmd, int *fd, t_param *param)
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	iterate_ast((t_cmd *)pipe_cmd->right, param);
	exit(0);
}

int	set_pipe(t_pipe *pipe_cmd, t_param *param)
{
	int fd[2];
	pid_t pid_l;
	pid_t pid_r;
	int status;

	if (pipe(fd) == -1)
		return (-1);
	pid_l = fork();
	if (pid_l == 0)
	{
		left_pipe(pipe_cmd, fd, param);
	}
	pid_r = fork();
	if (pid_r == 0)
	{
		right_pipe(pipe_cmd, fd, param);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid_l, NULL, 0);
	waitpid(pid_r, &status, 0);
	return (0);
}