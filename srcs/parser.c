#include "../includes/minishell.h"

void iterate_ast(t_cmd *cmd, t_list *env_list)
{
	if (cmd == NULL)
		return;
	if (cmd->type == PIPE)
	{
		t_pipe *pipe_cmd = cmd->cmd.pipe;
		set_pipe(pipe_cmd, env_list);
	} 
	else if (cmd->type == REDIR)
	{
		t_redir *redir_cmd = cmd->cmd.redir;
		set_redir(redir_cmd, env_list);
	}
	else if (cmd->type == EXEC)
	{
		t_exec *exec_cmd = cmd->cmd.exec;
		set_exec(exec_cmd, env_list);
	} 
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
		if(ft_strcmp(tokens[i], "<") == 0 || ft_strcmp(tokens[i], ">") == 0 || ft_strcmp(tokens[i], ">>") == 0)
		{
			fd = ft_strcmp(tokens[i], "<") == 0 ? 0 : 1;
			// fd = ft_strcmp(tokens[i], ">>") == 0 ? 2 : 1;
			free(tokens[i]);
			tokens[i] = NULL;
			file_name = ft_strdup(tokens[i + 1]);
			if (file_name == NULL)
			{
				printf("syntax error near unexpected token '|'\n");
				return (NULL);
			}
			i += 1;
			printf("%s %d\n", tokens[i], fd);

			if (!command)
				command = parse_exec(tokens); // Parse the command if it's not done yet
			if (command)
				command = construct_redir(command, fd, file_name); // Add redirection
			free(file_name);
		}
		else if(ft_strcmp(tokens[i], "<<") == 0)
			command = parse_here_doc(command, tokens, i);
		i++;
	}
	if (!command) // If there is no redirection -> parse a simple exec command
		command = parse_exec(tokens);
	return command;
}

int check_invalid_syntax(char **tokens, int i)
{
	if (tokens[i + 1] == NULL)
	{
		printf("syntax error near unexpected token '|'\n");
		return (1);
	}
	if (ft_strcmp(tokens[i + 1], "|") == 0)
	{
		printf("Bad syntax, out of scope of minishell\n");
		return (1);
	}
	else if (tokens[i - 1] != NULL && 
		(ft_strcmp(tokens[i - 1], "<") == 0 || ft_strcmp(tokens[i - 1], ">") == 0))
	{
		printf("Syntax error near unexpected token '|' \n");
		return (1);
	}
	else if (tokens[i - 1] != NULL && 
		(ft_strcmp(tokens[i - 1], "<<") == 0 || ft_strcmp(tokens[i - 1], ">>") == 0))
	{
		printf("Syntax error near unexpected token '|' \n");
		return (1);
	}
	return 0;
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
			if(check_invalid_syntax(tokens, i))
				return NULL;
			free(tokens[i]);
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
	ast = parse_redir(tokens);
	return(ast);
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
