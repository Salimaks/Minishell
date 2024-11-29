/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 16:08:23 by skassimi          #+#    #+#             */
/*   Updated: 2023/11/21 12:18:33 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	free_words(char **str, int size)
{
	while (size--)
		free(str[size]);
	return (0);
}

static int	count_str(char const *s, char c)
{
	int	i;
	int	words;

	words = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if ((s[i + 1] == c || s[i + 1] == '\0') == 1
			&& (s[i] == c || s[i] == '\0') == 0)
			words++;
		i++;
	}
	return (words);
}

static void	write_word(char *dest, const char *from, char charset)
{
	int	i;

	i = 0;
	while ((from[i] == charset || from[i] == '\0') == 0)
	{
		dest[i] = from[i];
		i++;
	}
	dest[i] = '\0';
}

static int	split_words(char **split, const char *str, char charset)
{
	int	i;
	int	j;
	int	word;

	word = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if ((str[i] == charset || str[i] == '\0') == 1)
			i++;
		else
		{
			j = 0;
			while ((str[i + j] == charset || str[i + j] == '\0') == 0)
				j++;
			split[word] = (char *)malloc(sizeof(char) * (j + 1));
			if (!split[word])
				return (free_words(split, word - 1));
			write_word(split[word], str + i, charset);
			i += j;
			word++;
		}
	}
	return (1);
}

char	**ft_split(const char *s, char c)
{
	char	**str;
	int		words;

	words = count_str(s, c);
	str = (char **)malloc(sizeof(char *) * (words + 1));
	if (!str)
		return (NULL);
	str[words] = 0;
	if (!split_words(str, s, c))
		return (NULL);
	return (str);
}
/*int main() {
    const char *s = "Hello,World,How,Are,You,";
    char c = ',';
    char **result = ft_split(s, c);

    if (result != NULL) {
        for (int i = 0; result[i] != NULL; i++) {
            printf("Mot %d: %s\n", i + 1, result[i]);
        }
    } else {
        printf("Allocation de mémoire a échoué.\n");
    }
    return 0;
}*/