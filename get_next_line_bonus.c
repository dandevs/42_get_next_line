#include "fcntl.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"
#include "get_next_line.h"

#ifndef BUFFSER_SIZE
# define BUFFER_SIZE 5
#endif

int	str_append(char **str_ptr, int str_len, char *to_append, int max_length)
{
	char	*str;
	char	*temp;
	int		i;
	int		j;

	str = *str_ptr;
	temp = malloc(str_len + max_length + 1);
	i = 0;
	if (!temp)
		return (-1);
	while (str[i])
	{
		temp[i] = str[i];
		i++;
	}
	j = 0;
	while (to_append[j] && j < max_length)
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
		{
			free(reader);
			return (NULL);
		}
		reader->fd = fd;
		reader->buffer_start = reader->buffer;
		reader->bytes_left = read(fd, reader->buffer, BUFFER_SIZE);
		reader->bytes_read = 0;
		cache[fd] = reader;
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

int	line_reader_read(t_line_reader *reader, char **line)
{
	int	i;

	while (line_reader_consume(reader, 0)->bytes_left > 0)
	{
		i = 0;
		while (i < reader->bytes_left && reader->buffer[i] != '\n')
			i++;
		if (reader->buffer[i] == '\n')
		{
			if (!str_append(line, reader->bytes_read, reader->buffer, i + 1))
				return (0);
			line_reader_consume(reader, i + 1);
			return (1);
		}
		else
		{
			str_append(line, reader->bytes_read, reader->buffer, i);
			line_reader_consume(reader, i);
		}
	}
	return (reader->bytes_left >= 0);
}

char	*read_line(int fd)
{
	t_line_reader	*reader;
	char			*line;

	if (fd < 0 || fd >= 4096 || BUFFER_SIZE <= 0)
		return (NULL);
	line = malloc(sizeof(char));
	if (!line)
		return (NULL);
	reader = get_line_reader(fd);
	if (!reader)
		return (NULL);
	*line = 0;
	if (line_reader_read(reader, &line))
		return (line);
	else
	{
		free(reader);
		free(line);
		return (NULL);
	}
}

int	main(void)
{
	int fd_0 = open("foo.txt", O_RDONLY);

	printf("%s", read_line(fd_0));
	printf("%s", read_line(fd_0));
	printf("%s", read_line(fd_0));
	printf("%s", read_line(fd_0));
	printf("%s", read_line(fd_0));
	printf("%s", read_line(fd_0));
	return (0);
}
