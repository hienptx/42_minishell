/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:23:45 by mafalda           #+#    #+#             */
/*   Updated: 2024/10/27 18:05:34 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	invalid_syntax_sms(char *s)
{
	printf("%s\n", s);
	return (1);
}

int	is_operator(const char *token)
{
	return (ft_strcmp(token, "&&") == 0 || ft_strcmp(token, "||") == 0
		|| ft_strcmp(token, ">") == 0 || ft_strcmp(token, "<") == 0
		|| ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0
		|| ft_strcmp(token, "|") == 0);
}

static int	is_pipe_op(const char *tok)
{
	return (ft_strcmp(tok, "||") == 0 || ft_strcmp(tok, "|") == 0);
}

static int	is_redir_op(const char *tok)
{
	return (ft_strcmp(tok, ">") == 0 || ft_strcmp(tok, "<") == 0
		|| ft_strcmp(tok, ">>") == 0 || ft_strcmp(tok, "<<") == 0);
}

int	check_syntax(char **tokens)
{
	int	i;

	i = 0;
	if ((is_operator(tokens[0]) && tokens[1] == NULL) || is_pipe_op(tokens[0]))
		return (invalid_syntax_sms("Syntax error"));
	while (tokens[i] != NULL)
	{
		if (is_redir_op(tokens[i]) && tokens[i + 1] == NULL)
			return (invalid_syntax_sms("Syntax error"));
		if (is_pipe_op(tokens[i]) && is_operator(tokens[i - 1]))
			return (invalid_syntax_sms("Syntax error"));
		if (is_redir_op(tokens[i]) && is_redir_op(tokens[i + 1]))
			return (invalid_syntax_sms("Syntax error"));
		i++;
	}
	return (0);
}
