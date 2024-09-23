#include "minishell.h"

int heredoc_process(char *arg)
{
	char *str;
	int		i;

	i = ft_strlen(arg);
    int hd_fd = open("here_doc.txt", O_CREAT);
	while (1)
	{
		ft_putstr_fd("> ", 0);
		str = get_next_line(0);
		if (str != NULL)
		{
			if (ft_strncmp(str, arg, ft_strlen(arg)) == 0 && str[i] == '\n')
			{
				free(str);
				break;
			}
			ft_putstr_fd(str, hd_fd);
			free(str);
		}
	}
    return(hd_fd);
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
        printf("Fd: %i\n", (redir_cmd->fd == 0) ? 0 : 1);
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
    t_cmd *command;
    int fd;
    char *file_name;

    i = 0;
    command = NULL;
    while(tokens[i] != NULL)
    {
        if(ft_strcmp(tokens[i], "<") == 0 || ft_strcmp(tokens[i], ">") == 0)
        {
            fd = ft_strcmp(tokens[i], "<") == 0 ? 00 : 01;
            file_name = tokens[i + 1];
            tokens[i] = NULL; //set string "<" to NULL
            if (!command)
                command = parse_exec(tokens);
            command = construct_redir(command, fd, file_name);
        }
        else if(ft_strcmp(tokens[i], "<<") == 0)
        {
            file_name = tokens[i + 1];
            if (file_name == NULL || ft_strcmp(file_name, "NULL") == 0)
            {
                printf("Bad syntax\n");
                exit(1);
            }
        //    fd = heredoc_process(file_name);
           tokens[i] = NULL;
        }
        i++;
    }
    if (!command) // if there is no redirection -> parse a simple exec command
        command = parse_exec(tokens);
    return (command);
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
