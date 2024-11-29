/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 13:06:24 by skassimi          #+#    #+#             */
/*   Updated: 2023/11/21 12:23:01 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		i;
	int		src_len;
	char	*arr;

	i = 0;
	src_len = 0;
	while (s[src_len])
		src_len++;
	arr = malloc(src_len + 1);
	if (!arr)
		return (NULL);
	while (s[i])
	{
		arr[i] = s[i];
		i++;
	}
	arr[i] = '\0';
	return (arr);
}
/*int main() {
    const char *original_str = "Hello, World!";

    char *copied_str = ft_strdup(original_str);

    if (copied_str != NULL) {
        printf("Original: %s\n", original_str);
        printf("Copied  : %s\n", copied_str);

        free(copied_str);
    } else {
        printf("Allocation de mémoire a échoué.\n");
    }

    return 0;
}*/
