#include "minishell.h"

int ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

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