#include "get_next_line_bonus.h"
#include <fcntl.h>

void	*multi_free(void *ptr_0, void *ptr_1)
{
	free(ptr_0);
	free(ptr_1);
	return (NULL);
}

char	**get_buffer_ptr(int fd)
{
	static char	**cache[4096];
	char		**buffer_ptr;
	char		*buffer;

	buffer_ptr = cache[fd];
	if (!buffer_ptr)
	{
		buffer_ptr = malloc(sizeof(char **) * 2);
		buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buffer_ptr || !buffer)
			return (multi_free(buffer_ptr, buffer));
		if (read(fd, buffer, BUFFER_SIZE) <= 0)
			return (multi_free(buffer_ptr, buffer));
		buffer[BUFFER_SIZE] = 0;
		buffer_ptr[0] = buffer;
		buffer_ptr[1] = buffer;
		cache[fd] = buffer_ptr;
	}
	if (!*buffer_ptr)
		return (NULL);
	return (buffer_ptr);
}

int	read_line(int fd, char **buffer_ptr, char **line)
{
	int		i;
	char	*buffer;

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
			*buffer_ptr = buffer;
			return (1);
		}
		str_append(line, buffer, i);
		buffer = buffer_ptr[1];
		i = read(fd, buffer, BUFFER_SIZE);
		buffer[i] = 0;
		*buffer_ptr = buffer;
		if (i == 0)
		{
			free(buffer);
			*buffer_ptr = NULL;
			return (1);
		}
		if (i < 0)
			return (0);
	}
}

char	*get_next_line(int fd)
{
	char			**buffer_ptr;
	char			*line;

	if (fd < 0 || fd >= 4096 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer_ptr = get_buffer_ptr(fd);
	if (!buffer_ptr)
	{
		free(buffer_ptr);
		return (NULL);
	}
	line = malloc(sizeof(char));
	if (!line)
		return (NULL);
	line[0] = 0;
	if (!read_line(fd, buffer_ptr, &line))
		return (multi_free(buffer_ptr, line));
	return (line);
}

// int	main(void)
// {
// 	int fd_0 = open("foo.txt", O_RDONLY);
// 	printf("%s", get_next_line(fd_0));
// 	printf("%s", get_next_line(fd_0));
// 	printf("%s", get_next_line(fd_0));
// 	printf("%s", get_next_line(fd_0));
// 	return (0);
// }
