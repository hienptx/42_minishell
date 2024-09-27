#include "../includes/minishell.h"
#include <stdarg.h>

void    left_pipe(t_pipe *pipe_cmd, int *fd, t_list *env_list)
{
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);
    iterate_ast((t_cmd *)pipe_cmd->left, env_list);
}

void    right_pipe(t_pipe *pipe_cmd, int *fd, t_list *env_list)
{
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    iterate_ast((t_cmd *)pipe_cmd->right, env_list);
}

int get_full_len(va_list args, const char *delimiter)
{
    const char  *str;
    int         total_len;

    va_list args_copy;
    va_copy(args_copy, args);

    int first = 1;
    total_len = 0;
    while (1)
    {
        str = va_arg(args_copy, const char *);
        if (str == NULL)
            break;
        if (!first && delimiter != NULL) {
            total_len += ft_strlen(delimiter);
        }
        total_len += ft_strlen(str);
        first = 0;
    }
    va_end(args_copy);
    return (total_len);
}

char    *cp_strs(char *result, va_list args, const char *delimiter)
{
    const char *str;
    char *ptr = result;
    int first = 1;

    while (1)
    {
        str = va_arg(args, const char *);
        if (str == NULL)
            break;
        if (!first && delimiter != NULL)
        {
            strcpy(ptr, delimiter);     //
            ptr += ft_strlen(delimiter);
        }
        strcpy(ptr, str);   //
        ptr += ft_strlen(str);
        first = 0;
    }
    *ptr = 0;
    return (ptr);
}

char    *ft_strsjoin(const char *delimiter, ...)
{
    va_list args;
    int     full_len;
    char    *ret;

    va_start(args, delimiter);
    full_len = get_full_len(args, delimiter);
    if (full_len == -1)
        return (NULL);
    ret = (char *)malloc(full_len + 1);
    if (ret == NULL) {
        va_end(args);
        return NULL;
    }
    cp_strs(ret, args, delimiter);
    va_end(args);
    return (ret);
}

int set_pipe(t_pipe *pipe_cmd, t_list *env_list)
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
        left_pipe(pipe_cmd, fd, env_list);
    }
    pid_r = fork();
    if (pid_r == 0)
    {
        right_pipe(pipe_cmd, fd, env_list);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid_l, NULL, 0);
    waitpid(pid_r, &status, 0);
    return (0);
}

void    set_redir(t_redir *redir_cmd, t_list *env_list)
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
    else if (redir_cmd->fd == 2)
    {
        redir_cmd->fd = open(redir_cmd->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (redir_cmd->fd == -1)
            exit(1);
        dup2(redir_cmd->fd, STDOUT_FILENO);
        close(redir_cmd->fd);
    }
    run_exec(redir_cmd->cmd, env_list);
}

char    **get_all_path(char *env_path)
{
    if (env_path == NULL)
        return (NULL);
    env_path += 5;

    // printf("gap %s\n", env_path);
    return(ft_split(env_path, ':'));
}

char    *get_executable_path(char *env_path, char *prog_name)
{
    char    **cur_dir;
    char    *executable_path;
    char    *ret;
    size_t  i;

    i = 0;
    // printf("gep %s\n", env_path);
    cur_dir = get_all_path(env_path);
    if (cur_dir == NULL)
        return (NULL);
    // printf("%d\n", 2);
    while (cur_dir[i])
    {
        // printf("%s\n", cur_dir[i]);
        executable_path = ft_strsjoin(NULL, cur_dir[i], "/", prog_name, NULL);
        // printf("the path %s\n", executable_path);
        if (executable_path == NULL)
            return (NULL);
        if (access(executable_path, X_OK) == 0)
        {
            ret = ft_strdup(executable_path);
            // free_2d_arr(cur_dir);
            return (ret);
        }
        i++;
    }
    return (NULL);
}

int call_exec(t_exec *exec_cmd, t_list *env_list)
{
    extern char **environ;
    char        *path;
    int         path_exist;

    path_exist = find_env(env_list, "PATH");
    // printf("path_exist %d\n", path_exist);
    if (path_exist == 1)
    {
        // printf("call exec%s\n", get_env_value("PATH", env_list));
        path = get_executable_path(get_env_value("PATH", env_list), exec_cmd->arg[0]);
        printf("%s\n", path);
        if (path == NULL)
            return (-1);
        exec_cmd->arg[0] = path;
        environ = mk_env_list(env_list);
        execve(path, exec_cmd->arg, environ);
        return (-2);
    }
    else if (path_exist == 0)
        return (0);
    else
        return (-1);
}

void    run_exec(t_exec *exec_cmd, t_list *env_list)
{
    pid_t   pid;
    int     status;

    // printf("%s\n", "run_exec\n");
    pid = fork();
    if (pid == 0)
        call_exec(exec_cmd, env_list);
    else
        waitpid(pid, &status, 0);
    return ;
}

void iterate_ast(t_cmd *cmd, t_list *env_list) 
{
    if (cmd == NULL) 
        return;
    if (cmd->type == PIPE)
    {
        t_pipe *pipe_cmd = cmd->cmd.pipe;
        set_pipe(pipe_cmd, env_list);
        // printf("Left pipe, level %d \n", level);
        // print_command_tree((t_cmd *)pipe_cmd->left, level + 1);
        // printf("Right pipe, level %d \n", level);
        // print_command_tree((t_cmd *)pipe_cmd->right, level + 1);
    } 
    else if (cmd->type == REDIR) 
    {
        t_redir *redir_cmd = cmd->cmd.redir;
        set_redir(redir_cmd, env_list);
        // printf("Redirection file name: %s\n", redir_cmd->file_name);
        // printf("Fd: %i\n", redir_cmd->fd);
        // print_command_tree((t_cmd *)redir_cmd->cmd, level + 1);
    } 
    else if (cmd->type == EXEC) 
    {
        t_exec *exec_cmd = cmd->cmd.exec;
        // printf("    Exec Command: ");
        if (ck_builtin(exec_cmd->arg[0]) == 1)
            call_builtin(exec_cmd, env_list);
        else
            run_exec(exec_cmd, env_list);
        // for (int i = 0; exec_cmd->arg[i] != NULL; i++)
        //     printf("%s ", exec_cmd->arg[i]);
        // printf("\n");
    } 
    // else 
        // printf("Unknown Command Level %d:\n", level);
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
