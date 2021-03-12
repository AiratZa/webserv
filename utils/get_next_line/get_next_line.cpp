/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 15:09:44 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/26 21:10:25 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.hpp"

int		end_handler(int fd, t_fd **start, char *buf, int return_value)
{
	t_fd *tmp;
	t_fd *tmp_to_del;

	free(buf);
	if (*start)
	{
		tmp = *start;
		if (tmp->fd == fd)
			*start = tmp->next;
		else
		{
			while (tmp->next->fd != fd)
				tmp = tmp->next;
			tmp_to_del = tmp->next;
			tmp->next = tmp_to_del->next;
			tmp = tmp_to_del;
		}
		free(tmp->left);
		free(tmp->left_begin);
		free(tmp);
		tmp = NULL;
	}
	return (return_value);
}

char	*left_checker(t_fd *tmp, char **line)
{
	char	*end_l;

	end_l = NULL;
	if (tmp->left)
	{
		if ((end_l = ft_strchr(tmp->left, '\n')))
		{
			*end_l = '\0';
			*line = ft_strdup(tmp->left);
			tmp->left = ++end_l;
		}
		else
		{
			*line = ft_strdup(tmp->left);
			free(tmp->left_begin);
			tmp->left = NULL;
			tmp->left_begin = NULL;
		}
		return (end_l);
	}
	if ((*line = static_cast<char *>(malloc(sizeof(char)))))
		**line = '\0';
	return (end_l);
}

int		g_n_l_deep(char **e_l, char **buf, t_fd *tmp, char **line)
{
	if ((*e_l = ft_strchr(*buf, '\n')))
	{
		**e_l = '\0';
		if (!(tmp->left = ft_strdup(++*e_l)))
			return (-1);
		else
			tmp->left_begin = tmp->left;
	}
	if (!(*line = ft_strjoin_re(line, *buf)))
		return (-1);
	return (0);
}

int		g_n_l_inside(t_fd *tmp, char **line, long long b_s, t_fd **start)
{
	char		*buf;
	long long	ret;
	char		*end_l;

    buf = static_cast<char *>(malloc(sizeof(char) * (b_s + 1)));
    if (!buf)
		return (end_handler(tmp->fd, start, buf, -1));
	end_l = left_checker(tmp, line);
	ret = 0;
	if (!(*line))
		return (end_handler(tmp->fd, start, buf, -1));
	while (!end_l && !(tmp->left) && (ret = read(tmp->fd, buf, b_s)))
	{
		if (ret < 0)
			return (end_handler(tmp->fd, start, buf, -1));
		buf[ret] = '\0';
		if (g_n_l_deep(&end_l, &buf, tmp, line) == -1)
			return (end_handler(tmp->fd, start, buf, -1));
	}
	if ((ret < b_s) && (tmp->left == NULL))
		return (end_handler(tmp->fd, start, buf, 0));
	free(buf);
	return (1);
}

int		get_next_line(int fd, char **line)
{
	static t_fd	*start;
	t_fd		*tmp;
	int	buf_size = 100;

	if (fd < 0 || line == NULL)
		return (-1);
	if (start == NULL)
		if (!(start = new_fd_to_read(fd)))
			return (-1);
	tmp = start;
	while ((tmp->fd != fd) && (tmp->next != NULL))
		tmp = tmp->next;
	if (tmp->fd != fd)
	{
		if (!(tmp->next = new_fd_to_read(fd)))
			return (-1);
		else
			tmp = tmp->next;
	}
	return (g_n_l_inside(tmp, line, buf_size, &start));
}
