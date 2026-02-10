/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:45:39 by danimend          #+#    #+#             */
/*   Updated: 2026/02/10 12:46:31 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_gnl	*get_gnl(void)
{
	static char		b_ptr[BUFFER_SIZE + 2];
	static t_gnl	gnl = {
		.buffer_start = b_ptr,
		.buffer = b_ptr
	};

	return (&gnl);
}

int	append_to_str(char **line, int line_len, char *app, int app_len)
{
	char	*str;
	int		i;
	int		j;

	str = malloc(sizeof(char) * (line_len + app_len + 1));
	if (!str)
		return (-1);
	i = 0;
	j = 0;
	while (i < line_len)
	{
		str[i] = (*line)[i];
		i++;
	}
	while (j < app_len)
		str[i++] = app[j++];
	str[i] = 0;
	free(*line);
	*line = str;
	return (i);
}

char	*refill_buffer(int fd, t_gnl *gnl, char *line)
{
	int	rd;

	rd = read(fd, gnl->buffer_start, BUFFER_SIZE);
	if (rd < 0)
	{
		free(line);
		return (NULL);
	}
	else if (rd == 0)
	{
		if (line && *line)
			return (line);
		free(line);
		return (NULL);
	}
	gnl->buffer_start[rd] = 0;
	gnl->buffer = gnl->buffer_start;
	return ((void *)-1);
}

char	*iterate_next_line_buffer(t_gnl *gnl, char **line, int *len)
{
	int	i;

	i = 0;
	while (gnl->buffer[i] && gnl->buffer[i] != '\n')
		i++;
	if (gnl->buffer[i] == '\n')
	{
		append_to_str(line, *len, gnl->buffer, i + 1);
		gnl->buffer += i + 1;
		return (*line);
	}
	append_to_str(line, *len, gnl->buffer, i);
	gnl->buffer += i;
	*len += i;
	return ((void *)-1);
}
