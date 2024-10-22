/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:59:41 by hipham            #+#    #+#             */
/*   Updated: 2024/10/22 20:59:32 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_substring(char *str, char *newsub, char *oldsub)
{
	char	*ret;
	char	*ptr;
	size_t	len;
	size_t	newsub_len;
	size_t	oldsub_len;

	newsub_len = ft_strlen(newsub);
	oldsub_len = ft_strlen(oldsub);
	len = ft_strlen(str) - oldsub_len + newsub_len;
	ret = ft_malloc(NULL, len + 1);
	ptr = ret;
	while (*str != '\0' && *str != '$')
		*ret++ = *str++;
	str = str + oldsub_len;
	while (*newsub != '\0')
		*ret++ = *newsub++;
	while (*str != '\0')
		*ret++ = *str++;
	*ret = '\0';
	return (ptr);
}

char	*get_expansion(char *substring, t_param *param, char *result)
{
	char	*ret_env;
	char	*ptr;
	int		is_allocated;

	ret_env = expand_str(substring + 1, param);
	is_allocated = (ret_env != NULL);
	if (ret_env == NULL)
	{
		ret_env = "";
		is_allocated = 0;
	}
	ptr = replace_substring(result, ret_env, substring);
	result = ptr;
	if (is_allocated)
		free(ret_env);
	return (result);
}

char	*expansion_loop(char *s, char *str, char *result, t_param *param)
{
	unsigned int	start;
	size_t			len;
	char			*substring;

	len = 0;
	while (s != NULL)
	{
		start = s - str;
		len = get_len(s[1]);
		if (s[1] != '?')
		{
			while (*s != '\0' && *s != ' ' && *s != '\'' && *s != '\"')
			{
				len++;
				s++;
			}
		}
		else
			s += 2;
		substring = ft_substr(str, start, len);
		result = get_expansion(substring, param, result);
		free(substring);
		s = ft_strchr(s, '$');
	}
	return (result);
}

char	*expansion_handling(char *str, t_param *param)
{
	char	*s;
	char	*result;

	s = ft_strchr(str, '$');
	if (s == NULL || (str[0] == '\'' && str[strlen(str) - 1] == '\''))
		return (str);
	if ((s[1] == ' ' || s[1] == '\0' || s[1] == '\"'))
		return (str);
	result = str;
	if (result == NULL)
		panic_sms("malloc", 1);
	result = expansion_loop(s, str, result, param);
	if (ft_strcmp(result, "") == 0)
	{
		free(result);
		return (NULL);
	}
	return (result);
}

char	*expand_str(char *str, t_param *param)
{
	char	*ret;

	if (str[0] == '?')
	{
		ret = ft_itoa(param->special.question_mark);
		if (ret == NULL)
			panic_sms("malloc", 1);
		return (ret);
	}
	else
		return (get_env_value(str, param->env_list));
}
