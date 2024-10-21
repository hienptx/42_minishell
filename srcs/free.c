/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 15:13:39 by mafalda           #+#    #+#             */
/*   Updated: 2024/10/21 23:00:44 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_param(t_param *param)
{
	param->env_list = NULL;
	param->special.question_mark = 0;
	set_env(&param->env_list);
	return ;
}

void	free_tokens(char **tokens, size_t nbr_tokens)
{
	size_t	i;

	i = 0;
	if (!tokens)
		return ;
	while (i < nbr_tokens)
	{
		if (tokens[i] != NULL)
		{
			free(tokens[i]);
		}
		i++;
	}
	free(tokens);
}

static void	free_redir(t_cmd *ast, t_redir *redir, t_redir *next)
{
	redir = ast->cmd.redir;
	while (redir != NULL)
	{
		next = redir->next;
		if (redir->file_name)
			free(redir->file_name);
		if (redir->cmd)
			free_ast((t_cmd *)redir->cmd);
		free(redir);
		redir = next;
	}
}
void	free_parse(t_parse_data parse)
{
	free_ast(parse.ast);
	free_tokens(parse.tok, parse.nbr_tokens);
	free(parse.input);
}

void	free_ast(t_cmd *ast)
{
	t_redir	*redir;
	t_redir	*next;

	next = NULL;
	redir = NULL;
	if (ast->type == PIPE)
	{
		free_ast(ast->cmd.pipe->left);
		free_ast(ast->cmd.pipe->right);
		free(ast->cmd.pipe);
	}
	else if (ast->type == REDIR)
	{
		free_redir(ast, redir, next);
	}
	else if (ast->type == EXEC)
	{
		free(ast->cmd.exec->arg);
		free(ast->cmd.exec);
	}
	free(ast);
}
