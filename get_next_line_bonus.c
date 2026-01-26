#include "get_next_line_bonus.h"
#include <fcntl.h>

char	*get_buffer(int fd)
{
	static char	*cache[4096];
	char		*buffer;
	int			rd;

	buffer = cache[fd];
	if (!buffer)
	{
		buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buffer)
			return (NULL);
		rd = read(fd, buffer, BUFFER_SIZE);
		if (rd <= 0)
		{
			free(buffer);
			return (NULL);
		}
		buffer[BUFFER_SIZE + 1] = 0;
		cache[fd] = buffer;
	}
	return (buffer);
}

int	read_line(int fd, char **buffer_ptr, char **line)
{
	int	i;
	char *buffer;

	buffer = *buffer_ptr;
	while (1)
	{
		i = 0;
		while (buffer[i] && buffer[i] != '\n')
			i++;
		if (buffer[i] == '\n')
		{
			str_append(line, buffer, i + 1);
			buffer += i + 1;
			return (1);
		}
		str_append(line, buffer, i);
		buffer += i;
		if (buffer[0] == 0)
			buffer -= BUFFER_SIZE;
		i = read(fd, buffer, BUFFER_SIZE);
		if (i == 0)
			return (1);
		if (i < 0)
			return (0);
	}
}

char	*get_next_line(int fd)
{
	char			*buffer;
	char			*line;

	if (fd < 0 || fd >= 4096 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = get_buffer(fd);
	if (!buffer)
		return (NULL);
	line = malloc(sizeof(char));
	if (!line)
		return (NULL);
	line[0] = 0;
	if (!read_line(fd, &buffer, &line))
	{
		free(buffer);
		free(line);
		return (NULL);
	}
	return (line);
}

int	main(void)
{
	int fd_0 = open("foo.txt", O_RDONLY);
	printf("%s", get_next_line(fd_0));
	printf("%s", get_next_line(fd_0));
	printf("%s", get_next_line(fd_0));
	return (0);
}
