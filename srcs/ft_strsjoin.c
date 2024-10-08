#include "../includes/minishell.h"
#include <stdarg.h>

char	*ft_strsjoin(const char *delimiter, ...)
{
	va_list	args;
	int		full_len;
	char	*ret;

	va_start(args, delimiter);
	full_len = get_full_len(args, delimiter);
	if (full_len == -1)
		return (NULL);
	ret = (char *)malloc(full_len + 1);
	if (ret == NULL)
	{
		va_end(args);
		return (NULL);
	}
	cp_strs(ret, args, delimiter);
	va_end(args);
	return (ret);
}

int	get_full_len(va_list args, const char *delimiter)
{
	const char	*str;
	int			total_len;
	va_list		args_copy;
	int			first;

	va_copy(args_copy, args);
	first = 1;
	total_len = 0;
	while (1)
	{
		str = va_arg(args_copy, const char *);
		if (str == NULL)
			break ;
		if (!first && delimiter != NULL)
		{
			total_len += ft_strlen(delimiter);
		}
		total_len += ft_strlen(str);
		first = 0;
	}
	va_end(args_copy);
	return (total_len);
}

char	*cp_strs(char *result, va_list args, const char *delimiter)
{
	const char	*str;
	char		*ptr;
	int			first;

	ptr = result;
	first = 1;
	while (1)
	{
		str = va_arg(args, const char *);
		if (str == NULL)
			break ;
		if (!first && delimiter != NULL)
		{
			ft_strcpy(ptr, delimiter);
			ptr += ft_strlen(delimiter);
		}
		ft_strcpy(ptr, str);
		ptr += ft_strlen(str);
		first = 0;
	}
	*ptr = 0;
	return (ptr);
}
