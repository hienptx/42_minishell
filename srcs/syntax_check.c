/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:23:45 by mafalda           #+#    #+#             */
/*   Updated: 2024/10/08 19:46:06 by hipham           ###   ########.fr       */
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
	return (ft_strcmp(token, "&&") == 0 || ft_strcmp(token, "||") == 0 ||
			ft_strcmp(token, ">") == 0 || ft_strcmp(token, "<") == 0 ||
			ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0 ||
			ft_strcmp(token, "|") == 0);
}

int	check_syntax(char **tokens)
{
	int	i;

	i = 0;
	if (tokens[0] != NULL && is_operator(tokens[0]))
		return (invalid_syntax_sms("Syntax error: unexpected token"));
	while (tokens[i] != NULL)
	{
		if (is_operator(tokens[i]))
		{
			if (tokens[i + 1] == NULL || is_operator(tokens[i + 1]))
				return (invalid_syntax_sms("Syntax error: unexpected token"));
		}
		if (ft_strcmp(tokens[i], "<") == 0 || ft_strcmp(tokens[i], ">") == 0 ||
			ft_strcmp(tokens[i], ">>") == 0 || ft_strcmp(tokens[i], "<<") == 0)
		{
			if (tokens[i + 1] == NULL || ft_strcmp(tokens[i + 1], "|") == 0
				|| is_operator(tokens[i + 1]))
				return (invalid_syntax_sms("Syntax error: unexpected token"));
		}
		i++;
	}
	return (0);
}
