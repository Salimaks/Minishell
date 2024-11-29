/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:33:31 by skassimi          #+#    #+#             */
/*   Updated: 2023/11/21 19:34:47 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*p1;
	const unsigned char	*p2;
	size_t				i;

	p1 = s1;
	p2 = s2;
	i = 0;
	while (i < n)
	{
		if (*p1 != *p2)
		{
			if (*p1 < *p2)
				return (-1);
			else
				return (1);
		}
		p1++;
		p2++;
		i++;
	}
	return (0);
}
/*int	main(void)
{
	char	s[50]="dede";
	char	a[6]="";

	printf("%d \n", ft_memcmp(s,a,6));
	printf("%d", memcmp(s,a,6));
	return (0);
}*/