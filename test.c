#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

typedef enum cmd_type 
{
    EXEC, // 0
    REDIR, // 1
    PIPE // 2
} t_type;

typedef struct exec 
{
    char **arg;
} t_exec;

typedef struct redir 
{
    t_exec *cmd;
    char *file_name;
    int flag;
    int fd;
} t_redir;

typedef struct pipe 
{
    char *left;
    char *right;
    void *next;
} t_pipe;

typedef struct s_cmd 
{
    t_type type;
    union 
    {
        t_pipe *pipecmd;
        t_redir *redircmd;
        t_exec *execcmd;
    } cmd;
} t_cmd;

t_cmd *parse_exec_command(char **tokens) 
{
    t_cmd *exec_cmd = malloc(sizeof(t_cmd));
    if (!exec_cmd)
        exit(EXIT_FAILURE);
    exec_cmd->type = EXEC;
    t_exec *exec_data = malloc(sizeof(t_exec));
    if (!exec_data)
        exit(EXIT_FAILURE);
    int arg_count = 0;
    while (tokens[arg_count] != NULL)
        arg_count++;
    exec_data->arg = malloc(sizeof(char *) * (arg_count + 1));
    for (int i = 0; i < arg_count; i++) 
        exec_data->arg[i] = strdup(tokens[i]);
    exec_data->arg[arg_count] = NULL;
    exec_cmd->cmd.execcmd = exec_data;
    return exec_cmd;
}

t_cmd *parse_simple_command(char **tokens) 
{
    t_cmd *command = NULL;
    t_cmd *redir_cmd = NULL;
    int i = 0;

    while (tokens[i] != NULL) 
    {
        if (strcmp(tokens[i], "<") == 0 || strcmp(tokens[i], ">") == 0) 
        {
            int redir_type = (strcmp(tokens[i], ">") == 0) ? O_WRONLY : O_RDONLY;
            char *file_name = tokens[i + 1];
            tokens[i] = NULL;
            if (!command) 
                command = parse_exec_command(tokens);
            redir_cmd = malloc(sizeof(t_cmd));
            if (!redir_cmd)
                exit(EXIT_FAILURE);
            redir_cmd->type = REDIR;
            redir_cmd->cmd.redircmd = malloc(sizeof(t_redir));
            if (!redir_cmd->cmd.redircmd)
                exit(EXIT_FAILURE);            
            redir_cmd->cmd.redircmd->cmd = (t_exec *)command;
            redir_cmd->cmd.redircmd->file_name = strdup(file_name);
            redir_cmd->cmd.redircmd->flag = redir_type;
            redir_cmd->cmd.redircmd->fd = (redir_type == O_WRONLY) ? 1 : 0;
            command = redir_cmd; // Update the command to reflect the latest redirection
            i++; // Skip the file name token
        }
        i++;
    }
    if (!command)
        command = parse_exec_command(tokens); // If no redirection, parse as a normal exec command
    return command;
}


t_cmd *parse_command(char **tokens)
{
    int i = 0;
    while (tokens[i] != NULL) 
    {
        if (strcmp(tokens[i], "|") == 0) 
        {
            tokens[i] = NULL;
            t_cmd *left = parse_command(tokens);
            t_cmd *right = parse_command(&tokens[i + 1]);
            if (!left || !right) 
                return NULL;
            t_cmd *pipe_cmd = malloc(sizeof(t_cmd));
            if (!pipe_cmd) 
                exit(EXIT_FAILURE);
            pipe_cmd->type = PIPE;
            pipe_cmd->cmd.pipecmd = malloc(sizeof(t_pipe));
            if (!pipe_cmd->cmd.pipecmd)
                exit(EXIT_FAILURE);
            pipe_cmd->cmd.pipecmd->left = (char *)left;
            pipe_cmd->cmd.pipecmd->right = (char *)right;
            // pipe_cmd->cmd.pipecmd->next = NULL;
            return pipe_cmd;
        }
        i++;
    }
    return parse_simple_command(tokens);
}

void print_command_tree(t_cmd *cmd, int level) 
{
    if (cmd == NULL) 
        return;
    if (cmd->type == PIPE)
    {
        t_pipe *pipe_cmd = cmd->cmd.pipecmd;
        printf("Pipe Command (Level %d)\n", level);
        printf("Left: \n");
        print_command_tree((t_cmd *)pipe_cmd->left, level + 1);
        printf("Right: \n");
        print_command_tree((t_cmd *)pipe_cmd->right, level + 1);
    } 
    else if (cmd->type == REDIR) 
    {
        t_redir *redir_cmd = cmd->cmd.redircmd;
        printf("Redirection file name: %s\n", redir_cmd->file_name);
        printf("Redirection Type: %s\n", (redir_cmd->flag == O_WRONLY) ? "Output" : "Input");
        print_command_tree((t_cmd *)redir_cmd->cmd, level + 1);
    } 
    else if (cmd->type == EXEC) 
    {
        t_exec *exec_cmd = cmd->cmd.execcmd;
        printf("Exec Command: ");
        for (int i = 0; exec_cmd->arg[i] != NULL; i++)
            printf("%s ", exec_cmd->arg[i]);
        printf("\n");
    } 
    else 
        printf("Unknown Command (Level %d):\n", level);
}

int main() 
{
    // char *tokens[] = {"sort", "-r", "<", "Makefile", "|", "wc -c", "|", "sleep 3", "|", "awk", ">","output.txt", NULL};
    char *tokens[] = {"cat", "Makefile", "<", "1 infile.txt", "<", "2 infile.txt",NULL};
    t_cmd *cmd_tree = parse_command(tokens);
    printf("Command structure:\n");
    print_command_tree(cmd_tree, 0);
    return 0;
}
