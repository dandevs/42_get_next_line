#include "get_next_line_bonus.h"
#include <fcntl.h>

static int	append_cause_newline(int len, int i, char **buffer_ptr, char **line)
{
	str_append(line, len, *buffer_ptr, i + 1);
	*buffer_ptr += i + 1;
	return (1);
}

static int	append_cause_eof(char **buffer_ptr, char **line, int fd)
{
	free(buffer_ptr[1]);
	free(buffer_ptr);
	get_cache()[fd] = (void *)-1;
	return (**line != 0);
}

int	read_line(int fd, char **buffer_ptr, char **line)
{
	int		i;
	char	*buffer;
	int		len;

	buffer = *buffer_ptr;
	len = 0;
	while (1)
	{
		i = 0;
		while (buffer[i] && buffer[i] != '\n')
			i++;
		if (buffer[i] == '\n')
			return (append_cause_newline(len, i, buffer_ptr, line));
		len += i;
		str_append(line, len, buffer, i);
		buffer = buffer_ptr[1];
		i = read(fd, buffer, BUFFER_SIZE);
		buffer[i] = 0;
		*buffer_ptr = buffer;
		if (i == 0)
			return (append_cause_eof(buffer_ptr, line, fd));
		if (i < 0)
			return (0);
	}
}

char	*get_next_line(int fd)
{
	char			**buffer_ptr;
	char			*line;

	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer_ptr = get_buffer_ptr(fd);
	if (!buffer_ptr)
	{
		free(buffer_ptr);
		return (NULL);
	}
	line = malloc(sizeof(char));
	if (!line)
		return (multi_free(buffer_ptr, line));
	line[0] = 0;
	if (!read_line(fd, buffer_ptr, &line))
		return (multi_free(buffer_ptr, line));
	return (line);
}

int	main(void)
{
	int fd = open("foo.txt", O_RDONLY);
	int i;

	for (i = 0; i < 10; i++)
	{
		char *line = get_next_line(fd);
		printf("%s", line);
		free(line);
	}
}