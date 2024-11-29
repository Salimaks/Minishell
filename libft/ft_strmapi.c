/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 09:25:41 by skassimi          #+#    #+#             */
/*   Updated: 2023/11/21 13:52:43 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	int		len;
	char	*str;

	i = 0;
	len = 0;
	while (s[len])
		len++;
	str = malloc(len + 1);
	if (!s || !f || !str)
		return (NULL);
	while (s[i])
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}
/*char m_function(unsigned int i, char c)
{   i = 1;
    return c + i;
}
int main()
{
    const char *str = "Hello, World!";
    char *result = ft_strmapi(str, &m_function);
    if (result != NULL)
    {
        printf("Original String: %s\n", str);
        printf("Mapped String  : %s\n", result);

        free(result);
    }
    else
    {
        printf("Memory allocation failed.\n");
    }
    return 0;
}*/
