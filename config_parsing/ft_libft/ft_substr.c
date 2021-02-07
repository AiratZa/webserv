/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 20:22:54 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/03 23:31:44 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	s_len;

	s_len = ft_strlen(s);
	if (s_len >= n)
	{
		dup = (char *)malloc((n + 1) * sizeof(char));
		if (dup == NULL)
			return (NULL);
		ft_strlcpy(dup, s, (n + 1));
		return (dup);
	}
	else
		return (ft_strdup(s));
}

char		*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;

	if (!s)
		return (NULL);
	if (ft_strlen(s) < start)
	{
		substr = (char *)malloc(sizeof(char));
		if (substr == NULL)
			return (NULL);
		*substr = '\0';
		return (substr);
	}
	else if (s)
		return (ft_strndup(&(s[start]), len));
	else
		return (NULL);
}
