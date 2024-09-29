#include "minishell.h"

// << here_doc
// >> output with appending option

char *walk_string(char *str, char c)
{
    str++;
    while (*str != '\0' && *str != c)
        str++;
    if (*str == c)
        str++;
    return str;
}

char *get_word(char *str)
{
    char *exceptor = "<>|";
 
    while (*str != '\0' && *str != ' ' && ft_strchr(exceptor, *str) == NULL)
    {
        if (*str == '\"')
            str = walk_string(str, '\"');
        else if (*str == '\'')
            str = walk_string(str, '\'');   
        else
            str++;
    }
    return(str);
}

size_t count_tokens(char *str)
{
    int count = 0;
    char *sep = "<>|";

    while (*str != '\0')
    {
        while (*str == ' ')
            str++;
        if (*str == '\0')
            break;
        if (ft_strchr(sep, *str) != NULL)
        {
            count++;
            if ((*str == '>' && *(str + 1) == '>') || (*str == '<' && *(str + 1) == '<'))
                str += 2; // Skip the '>>' or '<<'
            str++; // Skip the single special character
        }
        else
        {
            str = get_word(str);    
            count++;
        }
    }
    return count;
}

char *cpy_str(char **ret, char *str, size_t *pos)
{
    char *ptr;
    size_t wordlen;
    char *sep = "<>|";
    
    while (*str != '\0' && *str == ' ')
        str++;
    if (*str == '\0')
        return str;  // Return if the string ends after spaces
    ptr = str;
    if (ft_strchr(sep, *str) != NULL)
    {
        if ((*str == '>' && *(str + 1) == '>') || (*str == '<' && *(str + 1) == '<'))
            str += 2;
        else
            str++; // Length of single operator like '>', '<', '|'
        wordlen = str - ptr;  
    }
    else
    {
        str = get_word(str);
        wordlen = str - ptr;  // Calculate the word length
    }
    ret[*pos] = ft_malloc(ret[*pos], wordlen); //TO FREE
    ft_strncpy(ret[*pos], ptr, wordlen);
    ret[*pos][wordlen] = '\0';  // Null-terminate the string
    *pos += 1;
    return str;
}

// TODO check when input has redirection and pipe 
char **get_tokens(char *str, size_t *nbr_tokens)
{
    char **ret;
    char *ptr;
    // size_t nbr_tokens;
    size_t pos;

    ptr = str;
    *nbr_tokens = count_tokens(str);
    ret = malloc((*nbr_tokens + 1) * sizeof(char *));
    if (ret == NULL)
    {
        perror("Malloc failed to allocate memory\n");
        return NULL;
    }
    pos = 0;
    while (pos < *nbr_tokens)
        ptr = cpy_str(ret, ptr, &pos);
    ret[*nbr_tokens] = NULL;
    return (ret);
}
