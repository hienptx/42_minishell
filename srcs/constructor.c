/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:53:02 by hipham            #+#    #+#             */
/*   Updated: 2024/10/22 13:58:10 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*construct_pipe(char *left, char *right)
{
	t_cmd	*ast;

	ast = NULL;
	ast = malloc(sizeof(t_cmd));
	if (ast == NULL)
		panic_sms("Malloc failed", 1);
	ast->type = PIPE;
	ast->u_cmd.pipe = malloc(sizeof(t_pipe));
	if (ast->u_cmd.pipe == NULL)
		panic_sms("Malloc failed", 1);
	ast->u_cmd.pipe->left = left;
	ast->u_cmd.pipe->right = right;
	return (ast);
}

t_redir	*append_lst(t_redir *redir_list, char **tokens, char *filename,
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

t_cmd	*construct_redir(char **tokens, int fd, char *fn)
{
	t_cmd	*ast;

	ast = malloc(sizeof(t_cmd));
	if (ast == NULL)
		panic_sms("Malloc failed", 1);
	ast->type = REDIR;
	ast->u_cmd.redir = NULL;
	ast->u_cmd.redir = append_lst(ast->u_cmd.redir, tokens, fn, fd);
	if (ast->u_cmd.redir == NULL)
		return (NULL);
	return (ast);
}

t_exec	*construct_exec(char **tokens, t_exec *data)
{
	int	arg_count;
	int	i;

	arg_count = 0;
	while (tokens[arg_count] != NULL)
		arg_count++;
	data->arg = malloc(sizeof(char *) * (arg_count + 1));
	if (data->arg == NULL)
		panic_sms("Malloc failed", 1);
	i = 0;
	while (i < arg_count)
	{
		data->arg[i] = tokens[i];
		i++;
	}
	data->arg[arg_count] = NULL;
	return (data);
}

char	**mk_env_list(t_list *env_list)
{
	t_list	*cur;
	size_t	len_env_list;
	char	**env_arr;
	size_t	i;

	i = 0;
	cur = env_list;
	len_env_list = ft_lstsize(env_list);
	env_arr = (char **)malloc((len_env_list + 1) * sizeof(char *));
	if (!env_arr)
		return (NULL);
	while (cur != NULL)
	{
		env_arr[i] = (char *)cur->content;
		cur = cur->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}
