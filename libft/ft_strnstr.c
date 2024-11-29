/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 16:22:56 by skassimi          #+#    #+#             */
/*   Updated: 2023/11/21 13:56:13 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s, const char *search, size_t len)
{
	size_t	i;
	size_t	j;
	char	*str;

	i = 0;
	j = 0;
	str = (char *)s;
	if (search[0] == '\0')
		return (str);
	while (str[i] && i < len)
	{
		if (str[i] == search[0])
		{
			j = 0;
			while (str[i + j] && search[j]
				&& str[i + j] == search[j] && i + j < len)
				j++;
			if (search[j] == '\0')
				return (str + i);
		}
		i++;
	}
	return (NULL);
}
/*
int	main(void)
{
	char	str[50]="Hi my name is Salima kassimi";
	char	to_find[9]="Salima";

	printf("%s\t",ft_strnstr(str,to_find,sizeof(str)));
	return (0);	
}*/