/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 19:05:15 by skassimi          #+#    #+#             */
/*   Updated: 2023/11/21 14:43:32 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_trim(char const c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static char	*empty_str(void)
{
	char	*str;

	str = malloc(sizeof(char));
	if (!str)
		return (NULL);
	ft_strlcpy(str, "", 1);
	return (str);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	int		len;
	char	*str;

	if (!s1 || !set)
		return (0);
	i = 0;
	while (is_trim(s1[i], set))
		i++;
	len = ft_strlen(s1);
	if (i == len)
		return (empty_str());
	j = len - 1;
	while (is_trim(s1[j], set))
		j--;
	str = malloc(j - i + 2);
	if (!str)
		return (NULL);
	ft_strlcpy(str, (s1 + i), (j - i + 2));
	return (str);
}
/*int main() {
    const char *s1 = "       Hello, World!   ";
    const char *set = " ,\t";

    char *result = ft_strtrim(s1, set);

    if (result != NULL) 
    {
        printf("Trimmed: \"%s\"\n", result);
        free(result);
    } else {
        printf("Allocation de mémoire a échoué.\n");
    }

    return 0;
}*/