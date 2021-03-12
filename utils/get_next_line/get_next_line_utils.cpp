/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 15:09:55 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/26 21:10:59 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.hpp"

t_fd	*new_fd_to_read(int fd)
{
	t_fd	*newOne;

    newOne = static_cast<t_fd *>(malloc(sizeof(t_fd)));
	if (!newOne)
		return (NULL);
    newOne->fd = fd;
    newOne->left = NULL;
    newOne->left_begin = NULL;
    newOne->next = NULL;
	return (newOne);
}

size_t	ft_strlen(const char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == c)
			return ((char*)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

char	*ft_strdup(char *s)
{
	char	*new_arr;
	int		i;

	i = 0;
	new_arr = static_cast<char *>(malloc(sizeof(char) * (ft_strlen(s) + 1)));
	if (new_arr == NULL)
		return (NULL);
	else
	{
		while (s[i])
		{
			new_arr[i] = s[i];
			i++;
		}
		new_arr[i] = '\0';
		return (new_arr);
	}
}

char	*ft_strjoin_re(char **s1, char *s2)
{
	char	*joined;
	size_t	j_size;
	size_t	i;

	if (!*s1 || !s2)
		return (NULL);
	i = 0;
	j_size = ft_strlen(*s1) + ft_strlen(s2) + 1;
	joined = static_cast<char *>(malloc(sizeof(char) * j_size));
	if (joined == NULL)
		return (NULL);
	j_size = 0;
	while ((*s1)[i])
		joined[j_size++] = (*s1)[i++];
	i = 0;
	while (s2[i])
		joined[j_size++] = s2[i++];
	joined[j_size] = '\0';
	free(*s1);
	return (joined);
}
