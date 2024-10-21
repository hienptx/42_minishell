/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 16:35:48 by hipham            #+#    #+#             */
/*   Updated: 2024/10/21 16:25:45 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_word(char *str)
{
	char	*exceptor;

	exceptor = "<>|";
	while (*str != '\0' && *str != ' ' && ft_strchr(exceptor, *str) == NULL)
	{
		if (*str == '\"')
		{
			str++;
			while (*str != '\0' && *str != '\"')
				str++;
			if (*str == '\"')
				str++;
		}
		else if (*str == '\'')
		{
			str++;
			while (*str != '\0' && *str != '\'')
				str++;
			if (*str == '\'')
				str++;
		}
		else
			str++;
	}
	return (str);
}

size_t	count_tokens(char *str)
{
	size_t	count;
	char	*sep;

	count = 0;
	sep = "<>|";
	while (*str != '\0')
	{
		while (*str == ' ')
			str++;
		if (*str == '\0')
			break ;
		if (count_operator_tokens(&str))
			count++;
		else if (ft_strchr(sep, *str) != NULL)
		{
			count++;
			str++;
		}
		else
		{
			str = get_word(str);
			count++;
		}
	}
	return (count);
}

char	*process_operator(char **str)
{
	if (ft_strncmp(*str, ">>", 2) == 0 || ft_strncmp(*str, "<<", 2) == 0)
		*str += 2;
	else if (ft_strchr("<>|", **str) != NULL)
		(*str)++;
	return (*str);
}

char	*cpy_str(char **ret, char *str, size_t *pos)
{
	char	*ptr;
	size_t	wordlen;

	while (*str != '\0' && *str == ' ')
		str++;
	if (*str == '\0')
		return (str);
	ptr = str;
	str = process_operator(&str);
	if (str == ptr)
		str = get_word(str);
	wordlen = str - ptr;
	ret[*pos] = ft_malloc(ret[*pos], wordlen);
	ft_strncpy(ret[*pos], ptr, wordlen);
	ret[*pos][wordlen] = '\0';
	*pos += 1;
	return (str);
}

char	**get_tokens(char *str, size_t *nbr_tokens)
{
	char	**ret;
	char	*ptr;
	size_t	pos;

	ptr = str;
	*nbr_tokens = count_tokens(str);
	ret = malloc((*nbr_tokens + 1) * sizeof(char *));
	if (ret == NULL)
	{
		perror("Malloc failed to allocate memory\n");
		return (NULL);
	}
	pos = 0;
	while (pos < *nbr_tokens)
		ptr = cpy_str(ret, ptr, &pos);
	ret[*nbr_tokens] = NULL;
	return (ret);
}
