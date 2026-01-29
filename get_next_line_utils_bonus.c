#include "get_next_line_bonus.h"

t_gnl	*get_gnl(int fd)
{
	static t_gnl_cache	cache[1024];

	if (fd < 0 || fd >= 1024)
		return (NULL);
	if (!cache[fd].initialized)
	{
		cache[fd].gnl.buffer_start = cache[fd].buffer;
		cache[fd].gnl.buffer = cache[fd].buffer;
		cache[fd].initialized = 1;
	}
	return (&cache[fd].gnl);
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
		gnl->buffer_start[BUFFER_SIZE + 1] = 1;
		free(line);
		return (NULL);
	}
	else if (rd == 0)
	{
		gnl->buffer_start[BUFFER_SIZE + 1] = 1;
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
