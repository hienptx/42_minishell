/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:53:02 by hipham            #+#    #+#             */
/*   Updated: 2024/10/21 18:46:41 by dongjle2         ###   ########.fr       */
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
	ast->cmd.pipe = malloc(sizeof(t_pipe));
	if (ast->cmd.pipe == NULL)
		panic_sms("Malloc failed", 1);
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
		panic_sms("Malloc failed", 1);
	ast->type = REDIR;
	ast->cmd.redir = NULL;
	ast->cmd.redir = append_redir_list(ast->cmd.redir, tokens, file_name, fd);
	if (ast->cmd.redir == NULL)
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
		env_arr[i] = (char *)cur->content; //
		// printf("%s\n", (char *)cur->content);	//
		cur = cur->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}
