/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:53:02 by hipham            #+#    #+#             */
/*   Updated: 2024/10/07 20:29:10 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_ast(t_cmd *ast)
{
	t_redir	*redir;
	t_redir	*next;

	// int i;
	next = NULL;
	if (ast->type == PIPE)
	{
		free_ast(ast->cmd.pipe->left);
		free_ast(ast->cmd.pipe->right);
		free(ast->cmd.pipe);
	}
	else if (ast->type == REDIR)
	{
		redir = ast->cmd.redir;
		while (redir != NULL)
		{
			next = redir->next;
			if (redir->file_name)
				free(redir->file_name);
			if (redir->cmd)
				free_ast((t_cmd *)redir->cmd);
			else
			{
				;
			}
			free(redir);
			redir = next;
		}
	}
	else if (ast->type == EXEC)
	{
		// i = 0;
		// while (ast->cmd.exec->arg[i++] != NULL)
		//     free(ast->cmd.exec->arg[i]);
		free(ast->cmd.exec->arg);
		free(ast->cmd.exec);
	}
	free(ast);
}

t_cmd	*construct_pipe(char *left, char *right)
{
	t_cmd	*ast;

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

t_redir	*append_redir_list(t_redir *redir_list, char **tokens, char *filename,
		int fd)
{
	t_redir	*newnode;
	t_redir	*tmp;

	newnode = malloc(sizeof(t_redir));
	if (newnode == NULL)
		return (NULL);
	newnode->fd = fd;
	newnode->file_name = ft_strdup(filename);
	newnode->cmd = (t_exec *)parse_exec(tokens);
	newnode->next = NULL;
	if (redir_list == NULL)
		return (newnode);
	tmp = redir_list;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = newnode;
	return (redir_list);
}

t_cmd	*construct_redir(char **tokens, int fd, char *file_name)
{
	t_cmd	*ast;

	ast = malloc(sizeof(t_cmd));
	if (ast == NULL)
		panic_sms("Malloc failed to allocate memory");
	ast->type = REDIR;
	ast->cmd.redir = NULL;
	ast->cmd.redir = append_redir_list(ast->cmd.redir, tokens, file_name, fd);
	if (ast->cmd.redir == NULL)
		return (NULL);
	return (ast);
}

// t_cmd *construct_redir(t_exec *command, int fd, char *file_name)
// {
//     t_cmd *ast;

//     ast = malloc(sizeof(t_cmd));
//     if (ast == NULL)
//         panic_sms("Malloc failed to callocate memory");
//     ast->type = REDIR;
//     ast->cmd.redir = malloc(sizeof(t_redir));
//     if (ast->cmd.redir == NULL)
//         panic_sms("Malloc failed to allocate memory");
//     ast->cmd.redir->cmd = command;
//     ast->cmd.redir->file_name = ft_strdup(file_name);
//     ast->cmd.redir->fd = fd;
//     return (ast);
// }

t_exec	*construct_exec(char **tokens, t_exec *data)
{
	int	arg_count;
	int	i;

	arg_count = 0;
	while (tokens[arg_count] != NULL)
		arg_count++;
	data->arg = malloc(sizeof(char *) * (arg_count + 1));
	if (data->arg == NULL)
		panic_sms("Malloc failed to allocate memory");
	i = 0;
	while (i < arg_count)
	{
		data->arg[i] = tokens[i];
		i++;
	}
	data->arg[arg_count] = NULL;
	return (data);
}
