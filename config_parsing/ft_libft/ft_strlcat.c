/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 23:07:33 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/03 21:33:19 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_i;
	size_t	src_i;

	dst_i = 0;
	src_i = 0;
	while (dst[dst_i] && (dst_i < size))
		dst_i++;
	while (src[src_i] && ((dst_i + src_i + 1) < size))
	{
		dst[dst_i + src_i] = src[src_i];
		src_i++;
	}
	if (dst_i < size)
		dst[dst_i + src_i] = '\0';
	return (dst_i + ft_strlen(src));
}
