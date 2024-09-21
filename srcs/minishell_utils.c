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
    size_t i = 0;
    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}

void panic_sms(char *s)
{
    perror(s);
    exit (1);
}