/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hienxuan93@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 12:43:21 by hipham            #+#    #+#             */
/*   Updated: 2023/05/08 19:24:58 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

char	*ft_strstr(char *str, char *to_find)
{
	int	i;
	int	j;

	i = 0;
	if (to_find[i] == '\0')
		return (str);
	while (str[i] != '\0')
	{
		j = 0;
		while (str[i + j] == to_find[j] && to_find[j] != '\0')
			{
				j++;
			}
		if (to_find[j] == '\0')
			return(&str[i]);
	i++;
	}
	return (0);
}

// int main(void)
// {
// 	char str[] = "dfsa";
// 	char to_find[] = "adf";

// 	printf("%s", strstr(str, to_find));
// 	return (0);
// }