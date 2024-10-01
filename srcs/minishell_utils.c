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

void free_tokens(char **tokens, size_t nbr_tokens)
{
	size_t i;

	i = 0;
	if (!tokens)
		return;
	while (i < nbr_tokens)
	{
		if (tokens[i] != NULL)
		{
			free(tokens[i]);
		}
		i++;
	}
	free(tokens);
}

void panic_sms(char *s)
{
	perror(s);
	exit (1);
}

static	int	ft_isspace(const int c)
{
	const char	*space;

	space = "\t\n\v\f\r ";
	while (*space)
	{
		if (c == *space++)
			return (1);
	}
	return (0);
}

int	only_space(char *input)
{
	while (*input != '\0')
	{
		if (ft_isspace(*input) == 0)
		// if (*input != ' ')
			return (0);
		input++;
	}
	return(1);
}