/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:23:45 by mafalda           #+#    #+#             */
/*   Updated: 2024/10/26 02:43:00 by dongjle2         ###   ########.fr       */
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
	if (ft_strcmp(tokens[0], "|") == 0)
		return (invalid_syntax_sms("Syntax error"));
	while (tokens[i] != NULL)
	{
		if (is_operator(tokens[i]))
		{
			if (tokens[i + 1] == NULL
				&& !(ft_strcmp(tokens[i], "|") == 0
					&& (ft_strcmp(tokens[i + 1], ">") == 0
						|| ft_strcmp(tokens[i + 1], ">>") == 0)))
			{
				return (invalid_syntax_sms("Syntax error"));
			}
		}
		i++;
	}
	return (0);
}
