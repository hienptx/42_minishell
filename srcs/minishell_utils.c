#include "../includes/minishell.h"

void *ft_malloc(void *data, size_t size)
{
    data = malloc((size + 1) * sizeof(data));
    if (data == NULL)
    {
        perror("Malloc failed to allocate memmory\n");
        return NULL;
    }
    return (data);
}

void free_tokens(char **tokens)
{
    size_t i;
    
    i = 0;
    if (!tokens)
        return;
    while (1)
    {
        if (tokens[i] != NULL)
            free(tokens[i]);
        if (tokens[i] == NULL && tokens[i + 1] != NULL)
            i++;
        else
            break;
        i++;
    }
    free(tokens);
}

void panic_sms(char *s)
{
    perror(s);
    exit (1);
}

int only_space(char *input)
{
    while (*input != '\0')
    {
        if (*input != ' ')  
            return (0);
        input++;
    }
    return(1);
}
