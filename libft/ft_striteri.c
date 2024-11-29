/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 10:02:29 by skassimi          #+#    #+#             */
/*   Updated: 2023/11/21 12:24:21 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	int		i;

	i = 0;
	if (!s || !f)
		return (NULL);
	while (s[i])
	{
		f(i, s + i);
		i++;
	}
	return (s);
}
/*void uppercase_even_indices(unsigned int index, char *c)
{
    if (index % 2 == 0)
    {
        if (*c >= 'a' && *c <= 'z')
        {
            *c = *c - 32;
        }
    }
}

int main()
{
    char str[] = "hello, world!";
    printf("Original String: %s\n", str);
    ft_striteri(str, &uppercase_even_indices);
    printf("Modified String: %s\n", str);
    return 0;
}
*/