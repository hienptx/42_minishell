#include "minishell.h"

char *replace_substring(char *str, char *newsub, char *oldsub)
{
    char *ret;
    char *ptr;
    size_t len;
    size_t newsub_len;
    size_t oldsub_len;

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

char *expansion_handling(char *str)
{
    size_t len;
    char *s;
    unsigned int start;
    char *path;
    char *ret_env;
    char *ptr;

    s = ft_strchr(str, '$');
    if (s == NULL || (str[0] == '\'' && str[strlen(str) - 1] == '\'')) 
        return str;
    start = s - str;
    len = 0;
    while (*s != '\0' && *s != ' ' && *s != '\'' && *s != '\"') 
    {
        len++;
        s++;
    }
    path = ft_substr(str, start, len);
    ret_env = getenv(path + 1);
    if (ret_env == NULL) 
        ret_env = "";
    ptr = replace_substring(str, ret_env, path);
    free(path);
    return ptr;
}
