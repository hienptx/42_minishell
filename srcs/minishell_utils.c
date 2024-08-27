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

char *ft_malloc(char *str, size_t size)
{
    str = malloc((size + 1) * sizeof(char));
    if (str == NULL)
    {
        perror("Malloc failed to allocate memmory\n");
        return NULL;
    }
    return (str);
}