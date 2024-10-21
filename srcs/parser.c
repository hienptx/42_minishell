/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 21:51:31 by hipham            #+#    #+#             */
/*   Updated: 2024/10/17 23:02:27 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	iterate_ast(t_cmd *cmd, t_param *param, t_parse_data parse)
{
	t_pipe	*pipe_cmd;
	t_redir	*redir_cmd;
	t_exec	*exec_cmd;

	if (cmd == NULL)
		return ;
	if (cmd->type == PIPE)
	{
		pipe_cmd = cmd->cmd.pipe;
		set_pipe(pipe_cmd, param, parse);
	}
	else if (cmd->type == REDIR)
	{
		redir_cmd = cmd->cmd.redir;
		set_redir(redir_cmd, param, parse);
	}
	else if (cmd->type == EXEC)
	{
		exec_cmd = cmd->cmd.exec;
		set_exec(exec_cmd, param, parse);
	}
}

t_cmd	*parse_exec(char **tokens)
{
	t_exec	*data;
	t_cmd	*ast;

	if (tokens == NULL)
		return (NULL);
	ast = malloc(sizeof(t_cmd));
	if (ast == NULL)
		panic_sms("Malloc failed", 1);
	ast->type = EXEC;
	data = malloc(sizeof(t_exec));
	if (data == NULL)
		panic_sms("Malloc failed", 1);
	ast->cmd.exec = construct_exec(tokens, data);
	return (ast);
}

t_cmd	*process_redir(t_cmd *command, char **tokens, int i)
{
	char	*file_name;
	int		fd;

	fd = get_fd(tokens[i]);
	free(tokens[i]);
	tokens[i] = NULL;
	file_name = ft_strdup(tokens[i + 1]);
	if (file_name == NULL)
		panic_sms("Syntax error", 0);
	i += 1;
	if (command == NULL)
		command = construct_redir(tokens, fd, file_name);
	else
		command->cmd.redir = append_redir_list(command->cmd.redir, tokens,
				file_name, fd);
	free(file_name);
	return (command);
}

t_cmd	*parse_redir(char **tokens)
{
	int		i;
	t_cmd	*command;

	i = 0;
	command = NULL;
	while (tokens[i] != NULL)
	{
		if (ft_strcmp(tokens[i], "<") == 0 || ft_strcmp(tokens[i], ">") == 0
			|| ft_strcmp(tokens[i], ">>") == 0)
		{
			command = process_redir(command, tokens, i);
		}
		else if (ft_strcmp(tokens[i], "<<") == 0)
			command = parse_here_doc(command, tokens, i);
		i++;
	}
	if (!command)
		command = parse_exec(tokens);
	return (command);
}

t_cmd	*parse_cmd(char **tokens)
{
	int		i;
	t_cmd	*left;
	t_cmd	*right;
	t_cmd	*ast;

	i = 0;
	while (tokens[i] != NULL)
	{
		if (ft_strcmp(tokens[i], "|") == 0)
		{
			free(tokens[i]);
			tokens[i] = NULL;
			left = parse_cmd(tokens);
			right = parse_cmd(&tokens[i + 1]);
			if (!left || !right)
				return (NULL);
			ast = construct_pipe((char *)left, (char *)right);
			return (ast);
		}
		i++;
	}
	ast = parse_redir(tokens);
	return (ast);
}

void	print_command_tree(t_cmd *cmd, int level)
{
	t_pipe	*pipe_cmd;
	t_redir	*redir_cmd;
	t_exec	*exec_cmd;

	if (cmd == NULL)
		return ;
	if (cmd->type == PIPE)
	{
		pipe_cmd = cmd->cmd.pipe;
		printf("Left pipe, level %d \n", level);
		print_command_tree((t_cmd *)pipe_cmd->left, level + 1);
		printf("Right pipe, level %d \n", level);
		print_command_tree((t_cmd *)pipe_cmd->right, level + 1);
	}
	else if (cmd->type == REDIR)
	{
		redir_cmd = cmd->cmd.redir;
		while (redir_cmd != NULL)
		{
			printf("Redirection file name: %s\n",
				redir_cmd->file_name ? redir_cmd->file_name : "NULL");
			printf("Fd: %i\n", redir_cmd->fd);
			redir_cmd = redir_cmd->next;
		}
		if (cmd->cmd.redir != NULL)
		{
			print_command_tree((t_cmd *)cmd->cmd.redir->cmd, level + 1);
		}
	}
	else if (cmd->type == EXEC)
	{
		exec_cmd = cmd->cmd.exec;
		printf("    Exec Command: ");
		for (int i = 0; exec_cmd->arg[i] != NULL; i++)
			printf("%s ", exec_cmd->arg[i]);
		printf("\n");
	}
	else
		printf("Unknown Command Level %d:\n", level);
}
