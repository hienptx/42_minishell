#include "minishell.h"

void free_ast(t_cmd *ast)
{
    int i;

    if (ast->type == PIPE)
    {
        free_ast(ast->cmd.pipe->left);
        free_ast(ast->cmd.pipe->right);
        free(ast->cmd.pipe);
    }
    else if (ast->type == REDIR)
    {
        free_ast((t_cmd *)ast->cmd.redir->cmd);
        free(ast->cmd.redir->file_name);
        free(ast->cmd.redir);
    }
    else if (ast->type == EXEC)
    {
        i = 0;
        while(ast->cmd.exec->arg[i] != NULL)
        {
            free(ast->cmd.exec->arg[i]);
            i++;
        }
        free(ast->cmd.exec->arg);
        free(ast->cmd.exec);
    }
    free(ast);
}

t_cmd *construct_pipe(char *left, char *right)
{
    t_cmd *ast;

    ast = NULL;
    ast = malloc(sizeof(t_cmd));
    if (ast == NULL)
        panic_sms("Malloc failed to allocate memory");
    ast->type = PIPE;
    ast->cmd.pipe = malloc(sizeof(t_pipe));
    if (ast->cmd.pipe == NULL)
        panic_sms("Malloc failed to allocate memory");
    ast->cmd.pipe->left = left;
    ast->cmd.pipe->right = right;
    return (ast);
}

t_cmd *construct_redir(t_cmd *command, int fd, char *file_name)
{
    t_cmd *ast;

    ast = malloc(sizeof(t_cmd));
    if (ast == NULL)
        panic_sms("Malloc failed to callocate memory");
    ast->type = REDIR;
    ast->cmd.redir = malloc(sizeof(t_redir));
    if (ast->cmd.redir == NULL)
        panic_sms("Malloc failed to allocate memory");
    ast->cmd.redir->cmd = (t_exec *)command;
    ast->cmd.redir->file_name = ft_strdup(file_name);
    ast->cmd.redir->fd = fd;
    return (ast);
}

t_exec *construct_exec(char **tokens, t_exec *data)
{
    int arg_count;
    int i;

    arg_count = 0;
    while (tokens[arg_count] != NULL)
        arg_count++;
    data->arg = malloc(sizeof(char *) * (arg_count + 1));
    if (data->arg == NULL)
        panic_sms("Malloc failed to allocate memory");
    i  = 0;
    while (i < arg_count)
    {
        data->arg[i] = ft_strdup(tokens[i]);
        i++;
    }
    data->arg[arg_count] = NULL;
    return(data);
}
