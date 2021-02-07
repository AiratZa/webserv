/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 14:20:53 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/03 14:34:20 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(const char *s, char (*f)(unsigned int, char))
{
	char			*s_f;
	unsigned int	i;

	if (!s || !f)
		return (NULL);
	i = 0;
	s_f = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	if (s_f == NULL)
		return (NULL);
	while (s[i])
	{
		s_f[i] = (*f)(i, s[i]);
		i++;
	}
	s_f[i] = '\0';
	return (s_f);
}
