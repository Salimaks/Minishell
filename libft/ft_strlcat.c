/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 15:15:12 by skassimi          #+#    #+#             */
/*   Updated: 2023/11/21 13:20:42 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (dst[i] && i < size)
		i++;
	j = i;
	if (size > 0)
	{
		while (src[i - j] != '\0' && i < size - 1)
		{
			dst[i] = src[i - j];
			i++;
		}
		if (j < size)
			dst[i] = '\0';
	}
	return (j + ft_strlen(src));
}
/*int	main(void)
{
	char	dest[50]="you";
	char	src[20]=" are the best";

	ft_strlcat(dest,src,6);
	printf("the source string;%s\n", src);
	printf("the copy string :%s", dest);
	return (0);
}*/
