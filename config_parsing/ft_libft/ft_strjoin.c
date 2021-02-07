/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 21:27:45 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/09 18:30:26 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	size_t	j_size;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	j_size = ft_strlen(s1) + ft_strlen(s2) + 1;
	joined = (char *)malloc(j_size * sizeof(char));
	if (joined == NULL)
		return (NULL);
	j_size = 0;
	while (s1[i])
		joined[j_size++] = s1[i++];
	i = 0;
	while (s2[i])
		joined[j_size++] = s2[i++];
	joined[j_size] = '\0';
	return (joined);
}
