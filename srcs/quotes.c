/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:04:45 by hipham            #+#    #+#             */
/*   Updated: 2024/10/03 17:04:47 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int unclosed_quote(char **token)
{
    int i;

    i = 0;
    while(token[i] != NULL)
    {
        if (token[i][0] == '\'')
        {
           if (ft_strchr(token[i], '\'') == ft_strrchr(token[i], '\''))
                return 1;
        }
        if(token[i][0] == '"')
        {
            if (ft_strchr(token[i], '"') == ft_strrchr(token[i], '"'))
                return 1;
        }
        i++;
    }
    return 0;
}

size_t get_strlen(char *token)
{
    char *ptr;
    int quote_char;
    size_t len;

    quote_char = 0;
    len = 0;
    ptr = token;
    while (*ptr != '\0')
    {
        if ((*ptr == '\'' || *ptr == '"') && quote_char == 0)
            quote_char = *ptr;
        else if (*ptr == quote_char)
            quote_char = 0;
        else
            len++;
        ptr++;
    }
    return (len);
}

char *quote_handling(char *token)
{
    char *str;
    size_t len;
    char quote_char;
    char *non_quote;
    char *ptr;

    ptr = token;
    if (!ft_strchr(ptr, '\'') && !ft_strchr(ptr, '\"'))
        return ptr;
    len = get_strlen(token);
    str = ft_malloc(NULL, len + 1); // Plus one for the null terminator
    non_quote = str;
    quote_char = 0;
    while (*ptr != '\0')
    {
        if ((*ptr == '\'' || *ptr == '"') && quote_char == 0)
            quote_char = *ptr;
        else if (*ptr == quote_char)
            quote_char = 0;
        else
            *str++ = *ptr;
        ptr++;
    }
    *str = '\0';
    return non_quote;
}
