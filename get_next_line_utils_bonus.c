#include "get_next_line_bonus.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	str_append(char **str_ptr, char *to_append, int append_count)
{
	char	*str;
	char	*temp;
	int		i;
	int		j;

	str = *str_ptr;
	temp = malloc(sizeof(char) * (ft_strlen(str) + append_count + 1));
	i = 0;
	if (!temp)
		return (0);
	while (str[i])
	{
		temp[i] = str[i];
		i++;
	}
	j = 0;
	while (j < append_count && to_append[j])
	{
		temp[i + j] = to_append[j];
		j++;
	}
	temp[i + j] = 0;
	free(str);
	*str_ptr = temp;
	return (1);
}

t_line_reader	*get_line_reader(int fd)
{
	static t_line_reader	*cache[4096];
	t_line_reader			*reader;

	reader = cache[fd];
	if (!reader)
	{
		reader = malloc(sizeof(t_line_reader));
		if (!reader)
			return (NULL);
		reader->buffer = malloc(BUFFER_SIZE);
		if (!reader->buffer)
			return (0);
		reader->fd = fd;
		reader->buffer_start = reader->buffer;
		reader->bytes_left = read(fd, reader->buffer, BUFFER_SIZE);
		reader->bytes_read = 0;
		cache[fd] = reader;
		return (reader);
	}
	return (reader);
}

t_line_reader	*line_reader_consume(t_line_reader *reader, int count)
{
	if (count == 0)
	{
		if (reader->bytes_left == 0)
		{
			reader->buffer = reader->buffer_start;
			reader->bytes_left = read(reader->fd, reader->buffer, BUFFER_SIZE);
		}
	}
	else
	{
		reader->buffer += count;
		reader->bytes_read += count;
		reader->bytes_left -= count;
	}
	return (reader);
}

// int	line_reader_read(t_line_reader *reader, char **line)
// {
// 	int	i;

// 	while (line_reader_consume(reader, 0)->bytes_left > 0)
// 	{
// 		i = 0;
// 		while (i < reader->bytes_left && reader->buffer[i] != '\n')
// 			i++;
// 		if (i < reader->bytes_left && reader->buffer[i] == '\n')
// 		{
// 			if (!str_append(line, reader->bytes_read, reader->buffer, i + 1))
// 				return (line_reader_free(&reader));
// 			line_reader_consume(reader, i + 1);
// 			return (1);
// 		}
// 		else
// 		{
// 			if (!str_append(line, reader->bytes_read, reader->buffer, i))
// 				return (0);
// 			line_reader_consume(reader, i);
// 		}
// 	}
// 	return (reader->bytes_left > 0);
// }

int	line_reader_free(t_line_reader **reader)
{
	if (*reader)
	{
		// free((*reader)->buffer_start);
		// free(*reader);
	}
	return (0);
}
