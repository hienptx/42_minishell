#include "../includes/minishell.h"

void    left_pipe(t_pipe *pipe_cmd, int *fd)
{
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);
    iterate_ast((t_cmd *)pipe_cmd->left);
}

void    right_pipe(t_pipe *pipe_cmd, int *fd)
{
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    iterate_ast((t_cmd *)pipe_cmd->right);
}

int set_pipe(t_pipe *pipe_cmd)
{
    int     fd[2];
    pid_t   pid_l;
    pid_t   pid_r;
    int     status;

    if (pipe(fd) == -1)
        return (-1);
    pid_l = fork();
    if (pid_l == 0)
    {
        left_pipe(pipe_cmd, fd);
    }
    pid_r = fork();
    if (pid_r == 0)
    {
        right_pipe(pipe_cmd, fd);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid_l, NULL, 0);
    waitpid(pid_r, &status, 0);
    return (0);
}

void    set_redir(t_redir *redir_cmd)
{
    if (redir_cmd->fd == 0)     //input_redir
    {
        redir_cmd->fd = open(redir_cmd->file_name, O_RDONLY);
        if (redir_cmd->fd == -1)
            exit(1);
        dup2(redir_cmd->fd, STDIN_FILENO);
        close(redir_cmd->fd);
    }
    else if (redir_cmd->fd == 1)
    {
        redir_cmd->fd = open(redir_cmd->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (redir_cmd->fd == -1)
            exit(1);
        dup2(redir_cmd->fd, STDOUT_FILENO);
        close(redir_cmd->fd);
    }
}

void print_command_tree(t_cmd *cmd, int level) 
{
    if (cmd == NULL) 
        return;
    if (cmd->type == PIPE)
    {
        t_pipe *pipe_cmd = cmd->cmd.pipe;
        printf("Left pipe, level %d \n", level);
        print_command_tree((t_cmd *)pipe_cmd->left, level + 1);
        printf("Right pipe, level %d \n", level);
        print_command_tree((t_cmd *)pipe_cmd->right, level + 1);
    } 
    else if (cmd->type == REDIR) 
    {
        t_redir *redir_cmd = cmd->cmd.redir;
        printf("Redirection file name: %s\n", redir_cmd->file_name);
        printf("Fd: %i\n", redir_cmd->fd);
        print_command_tree((t_cmd *)redir_cmd->cmd, level + 1);
    } 
    else if (cmd->type == EXEC) 
    {
        t_exec *exec_cmd = cmd->cmd.exec;
        printf("    Exec Command: ");
        for (int i = 0; exec_cmd->arg[i] != NULL; i++)
            printf("%s ", exec_cmd->arg[i]);
        printf("\n");
    } 
    else 
        printf("Unknown Command Level %d:\n", level);
}

t_cmd *parse_exec(char **tokens)
{
    t_cmd *ast;
    t_exec *data;

    if(tokens == NULL)
        return (NULL);
    ast = malloc(sizeof(t_cmd));
    if (ast == NULL)
        panic_sms("Malloc failed to allocate memory");
    ast->type = EXEC;
    data = malloc(sizeof(t_exec));
    if (data == NULL)
        panic_sms("Malloc failed to allocate memory");
    ast->cmd.exec = construct_exec(tokens, data);
    return (ast);
}

t_cmd *parse_redir(char **tokens)
{
    int i;
    t_cmd *command = NULL;
    int fd;
    char *file_name;

    i = 0;
    while(tokens[i] != NULL)
    {
        if(ft_strcmp(tokens[i], "<") == 0 || ft_strcmp(tokens[i], ">") == 0 || ft_strcmp(tokens[i], ">>") == 0)
        {
            fd = ft_strcmp(tokens[i], "<") == 0 ? 0 : 1;
            file_name = tokens[i + 1];
            tokens[i] = NULL; // set string "<", ">", or ">>" to NULL
            if (!command)
                command = parse_exec(tokens); // Parse the command if it's not done yet
            if (command)
                command = construct_redir(command, fd, file_name); // Add redirection
        }
        else if(ft_strcmp(tokens[i], "<<") == 0)
            command = parse_here_doc(command, tokens, i);
        i++;
    }
    if (!command) // If there is no redirection -> parse a simple exec command
        command = parse_exec(tokens);
    return command;
}


t_cmd *parse_cmd(char **tokens)
{
    int i;
    t_cmd *left;
    t_cmd *right;
    t_cmd *ast;

    i = 0;
    while(tokens[i] != NULL)
    {
        if (ft_strcmp(tokens[i], "|") == 0)
        {
            if (ft_strcmp(tokens[i + 1], "|") == 0)
            {
                printf("Bad syntax, out of scope of minishell\n");
                return (NULL);
            }
            tokens[i] = NULL;
            left = parse_cmd(tokens);
            right = parse_cmd(&tokens[i + 1]);
            if (!left || !right)
                return NULL;
            ast = construct_pipe((char *)left, (char *)right);
            return(ast);
        }
        i++;
    }
    return(parse_redir(tokens));
}       
