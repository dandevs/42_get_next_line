#include "fcntl.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"

#ifndef BUFFSER_SIZE
# define BUFFER_SIZE 42
#endif

typedef struct s_line_reader
{
	char	*buffer;
	char	*buffer_start;
	int		fd;
	int		bytes_left;
}	t_line_reader;

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	if (!str)
		return (0);
	while (str[len])
		len++;
	return (len);
}

int	str_append(char **str_ptr, char *to_append, int max_length)
{
	char	*str;
	char	*temp;
	int		i;
	int		j;

	str = *str_ptr;
	temp = malloc(ft_strlen(str) + max_length + 1);
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
	t_line_reader			*entry;

	entry = cache[fd];
	if (!entry)
	{
		entry = malloc(sizeof(t_line_reader));
		if (!entry)
			return (NULL);
		entry->fd = fd;
		entry->buffer = malloc(BUFFER_SIZE);
		if (!entry->buffer)
			return (NULL);
		entry->buffer_start = entry->buffer;
		entry->bytes_left = read(fd, entry->buffer, BUFFER_SIZE);
		cache[fd] = entry;
	}
	return (entry);
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
		reader->bytes_left -= count;
	}
	return (reader);
}

char	*line_reader_read(t_line_reader *reader)
{
	char	*line;
	int		i;

	line = malloc(sizeof(char));
	while (line_reader_consume(reader, 0)->bytes_left > 0)
	{
		i = 0;
		while (i < reader->bytes_left && reader->buffer[i] != '\n')
			i++;
		if (reader->buffer[i] == '\n')
		{
			if (!str_append(&line, reader->buffer, i + 1))
				return (NULL);
			line_reader_consume(reader, i + 1);
			return (line);
		}
		else
		{
			str_append(&line, reader->buffer, i);
			line_reader_consume(reader, i);
		}
	}
	if (reader->bytes_left == 0)
		return (line);
	free(line);
	return (NULL);
}

char	*read_line(int fd)
{
	t_line_reader	*reader;
	char			*line;
	int				i;

	if (fd < 0 || fd >= 4096)
		return (NULL);
	line = malloc(1);
	reader = get_line_reader(fd);
	if (!line || !reader)
		return (NULL);
	*line = 0;
	while (line_reader_consume(reader, 0)->fd > 0)
	{
		i = 0;
		while (i < reader->bytes_left && reader->buffer[i] != '\n')
			i++;
		if (reader->buffer[i] == '\n')
		{
			if (!str_append(&line, reader->buffer, i + 1))
				return (NULL);
			line_reader_consume(reader, i + 1);
			return (line);
		}
		else
		{
			str_append(&line, reader->buffer, i);
			line_reader_consume(reader, i);
		}
	}
	return (line);
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
