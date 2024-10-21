/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:23:45 by mafalda           #+#    #+#             */
/*   Updated: 2024/10/21 18:54:11 by dongjle2         ###   ########.fr       */
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

int	check_syntax(char **tokens)
{
	int	i;

	i = 0;
	if (tokens[0] != NULL && is_operator(tokens[0]))
		return (invalid_syntax_sms("Syntax error"));
	while (tokens[i] != NULL)
	{
		if (ft_strcmp(tokens[i], "||") == 0)
			return (invalid_syntax_sms("Syntax error"));
		if (is_operator(tokens[i]))
		{
			if (tokens[i + 1] == NULL || is_operator(tokens[i + 1]))
				return (invalid_syntax_sms("Syntax error"));
		}
		i++;
	}
	return (0);
}
