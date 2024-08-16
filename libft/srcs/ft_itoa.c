/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 18:05:19 by hipham            #+#    #+#             */
/*   Updated: 2023/12/26 17:28:07 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// #include <stdio.h>
// #include <unistd.h>

// static char	*to_reverse(char *ptr)
// {
// 	char	*start;
// 	char	*end;
// 	char	temp;
// 	size_t	len;

// 	if (ptr == NULL)
// 		return (NULL);
// 	len = ft_strlen(ptr);
// 	start = ptr;
// 	end = ptr + len - 1;
// 	while (start < end)
// 	{
// 		if (*start == '-')
// 			start++;
// 		temp = *start;
// 		*start = *end;
// 		*end = temp;
// 		start++;
// 		end--;
// 	}
// 	return (ptr);
// }

// static char	*ft_chopnum(long int num, char *ptr)
// {
// 	char	*temp;

// 	temp = ptr;
// 	while (num)
// 	{
// 		*temp = num % 10 + '0';
// 		num = num / 10;
// 		temp++;
// 	}
// 	*temp = '\0';
// 	return (ptr);
// }

static size_t	ft_countdigit(long int num)
{
	size_t	count;

	if (num <= 0)
		count = 1;
	else
		count = 0;
	while (num != 0)
	{
		num = num / 10;
		count++;
	}
	return (count);
}

// char	*ft_itoa(int n)
// {
// 	char		*ptr;
// 	char		*temp;
// 	long int	num;

// 	num = n;
// 	if (num == 0)
// 		return (ft_strdup("0"));
// 	ptr = (char *)malloc(ft_countdigit(num) + 1);
// 	if (ptr == NULL)
// 		return (NULL);
// 	temp = ptr;
// 	if (num < 0)
// 	{
// 		num = -num;
// 		*temp = '-';
// 		temp++;
// 	}
// 	ft_chopnum(num, temp);
// 	return (to_reverse(ptr));
// }

char	*ft_itoa(int n)
{
	long	num;
	char	*ptr;
	int		i;

	num = (long)n;
	i = ft_countdigit(n);
	ptr = (char *)malloc(i + 1);
	if (ptr == NULL)
		return (NULL);
	ptr[i] = '\0';
	if (num == 0)
		ptr[0] = '0';
	if (num < 0)
	{
		ptr[0] = '-';
		num = -num;
	}
	while (num)
	{
		ptr[i - 1] = num % 10 + '0';
		num = num / 10;
		i--;
	}
	return (ptr);
}

// int	main(void)
// {
// 	ft_itoa(-402);
// 	write(1, "\n", 1);
// 	ft_itoa(4605118);
// 	write(1, "\n", 1);
// 	ft_itoa(0);
// 	write(1, "\n", 1);
// 	ft_itoa(-2147483648);
// 	write(1, "\n", 1);
// 	ft_itoa(2147483647);
// 	return (0);
// }