/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 12:07:17 by skassimi          #+#    #+#             */
/*   Updated: 2023/11/20 15:33:26 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*p;

	if (!dest && !src)
		return (NULL);
	i = 0;
	p = dest;
	while (i < n)
	{
		*p++ = *(unsigned char *)src++;
		i++;
	}
	return (dest);
}
