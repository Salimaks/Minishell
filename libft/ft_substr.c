/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 14:10:41 by skassimi          #+#    #+#             */
/*   Updated: 2023/11/21 14:47:08 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	diff(size_t strlen, size_t len)
{
	if (strlen > len)
		return (len);
	else
		return (strlen);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	s_len;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
	{
		str = malloc(sizeof(char) * 1);
		str[0] = '\0';
		return (str);
	}
	s_len = diff(ft_strlen(s + start), len);
	str = malloc(sizeof(char) * (s_len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < s_len && s[i + start])
	{
		str[i] = s[i + start];
		i++;
	}
	str[i] = '\0';
	return (str);
}
/*int main() {
    const char *original = "Hello, World!";

    char *substring = ft_substr(original, 7, 5);

    if (substring != NULL) {
        printf("Original: %s\n", original);
        printf("Substring: %s\n", substring);

        free(substring);
    } else {
        printf("Indices invalides ou allocation de mémoire a échoué.\n");
    }

    return 0;
}*/