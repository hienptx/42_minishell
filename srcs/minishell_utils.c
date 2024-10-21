/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:28:01 by hipham            #+#    #+#             */
/*   Updated: 2024/10/21 15:11:00 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*ft_malloc(void *data, size_t size)
{
	data = malloc((size + 1) * sizeof(data));
	if (data == NULL)
	{
		perror("Malloc failed to allocate memmory\n");
		return (NULL);
	}
	return (data);
}

void	*panic_sms(char *s, int i)
{
	if (i == 0)
		perror(s);
	else if (i == 1)
	{
		perror(s);
		exit(1);
	}
	else if (i == 2)
	{
		printf("%s\n", s);
		exit(0);
	}
	return (NULL);
}

int	only_space(char *input)
{
	while (*input != '\0')
	{
		if (ft_isspace(*input) == 0)
			return (0);
		input++;
	}
	return (1);
}

int	get_fd(char *token)
{
	int	fd;

	if (ft_strcmp(token, "<") == 0)
		fd = 0;
	if (ft_strcmp(token, ">") == 0)
		fd = 1;
	if (ft_strcmp(token, ">>") == 0)
		fd = 2;
	return (fd);
}

size_t	count_non_operators(char **tokens)
{
	size_t	len;
	int		i;

	len = 0;
	i = 0;
	while (tokens[i] != NULL)
	{
		if (is_operator(tokens[i]) != 0)
			i += 2;
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}
