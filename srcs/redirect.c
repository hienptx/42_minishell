#include "../includes/minishell.h"

void    set_redir(t_redir *redir_cmd, t_list *env_list)
{
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);
    if (redir_cmd->fd == 0)     //input_redir
    {
        redir_cmd->fd = open(redir_cmd->file_name, O_RDONLY);
        if (redir_cmd->fd == -1)
            exit(1);
        dup2(redir_cmd->fd, STDIN_FILENO);
    }
    else if (redir_cmd->fd == 1)
    {
        redir_cmd->fd = open(redir_cmd->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (redir_cmd->fd == -1)
            exit(1);
        dup2(redir_cmd->fd, STDOUT_FILENO);
    }
    else if (redir_cmd->fd == 2)
    {
        redir_cmd->fd = open(redir_cmd->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (redir_cmd->fd == -1)
            exit(1);
        dup2(redir_cmd->fd, STDOUT_FILENO);
    }
    else
    {
        dup2(redir_cmd->fd, STDIN_FILENO);
    }
    sleep(1000000);
    close(redir_cmd->fd);
    iterate_ast((t_cmd *)redir_cmd->cmd, env_list);
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
}