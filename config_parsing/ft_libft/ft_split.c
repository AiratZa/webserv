/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 09:15:28 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/03 23:26:16 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	splitted_word_count(char const *s, char c)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (s[i] && (s[i] == c))
		i++;
	while (s[i])
	{
		while (s[i] && (s[i] != c))
			i++;
		count++;
		while (s[i] && (s[i] == c))
			i++;
	}
	return (count);
}

static char		*new_word_start(char const *s, char c)
{
	while (*s && (*s == c))
		s++;
	return ((char *)s);
}

static size_t	word_len(char const *s, char c)
{
	size_t i;

	i = 0;
	while (s[i] && (s[i] != c))
		i++;
	return (i);
}

static void		mem_clean(char **splitted, size_t word_c)
{
	while (word_c > 0)
	{
		word_c--;
		free(splitted[word_c]);
	}
	free(splitted[word_c]);
	free(splitted);
}

char			**ft_split(char const *s, char c)
{
	char	**splitted;
	size_t	word_c;
	size_t	word_c_iter;

	if (!s)
		return (NULL);
	word_c_iter = 0;
	word_c = splitted_word_count((char *)s, c);
	splitted = (char **)malloc((word_c + 1) * sizeof(char *));
	if (splitted == NULL)
		return (NULL);
	while (word_c_iter < word_c)
	{
		s = new_word_start(s, c);
		splitted[word_c_iter] = ft_substr(s, 0, word_len(s, c));
		if (splitted[word_c_iter] == NULL)
		{
			mem_clean(splitted, word_c_iter);
			return (NULL);
		}
		word_c_iter++;
		s += word_len(s, c);
	}
	splitted[word_c] = NULL;
	return (splitted);
}
