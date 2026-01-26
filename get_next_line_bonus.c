#include "get_next_line_bonus.h"
#include <fcntl.h>

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
		{
			str_append(line, len, buffer, i + 1);
			buffer += i + 1;
			len += i + 1;
			*buffer_ptr = buffer;
			return (1);
		}
		len += i;
		str_append(line, len, buffer, i);
		buffer = buffer_ptr[1];
		i = read(fd, buffer, BUFFER_SIZE);
		buffer[i] = 0;
		*buffer_ptr = buffer;
		if (i == 0)
		{
			free(buffer);
			*buffer_ptr = NULL;
			// if (**line == 0)
			// 	return (0);
			// return (1);
			return (**line != 0);
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
// 	char *results[32];
// 	int i;


// 	results[0] = get_next_line(fd_0);
// 	results[1] = get_next_line(fd_0);
// 	// for (i = 0; i < 32; i++)
// 	// {
// 	// 	results[i] = get_next_line(fd_0);
// 	// 	if (results[i])
// 	// 		printf("%s\n", results[i]);
// 	// }
	
// 	return (0);
// }
