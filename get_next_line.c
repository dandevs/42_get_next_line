#include "get_next_line.h"

// static int	append_to_str(char **line, int line_len, char *app, int app_len)
// {
// 	char	*str;
// 	int		i;
// 	int		j;

// 	str = malloc(sizeof(char) * (line_len + app_len + 1));
// 	if (!str)
// 		return (-1);
// 	i = 0;
// 	j = 0;
// 	while (i < line_len)
// 	{
// 		str[i] = (*line)[i];
// 		i++;
// 	}
// 	while (j < app_len)
// 		str[i++] = app[j++];
// 	str[i] = 0;
// 	free(*line);
// 	*line = str;
// 	return (i);
// }

static	int foo(int fd, char *buffer_start, char *buffer, char *line)
{
	int	fd;

	fd = read(fd, buffer_start, BUFFER_SIZE);
	if (fd < 0)
	{
		buffer_start[BUFFER_SIZE + 1] = 2;
		free(line);
		return (NULL);
	}
	else if (fd == 0)
	{
		buffer_start[BUFFER_SIZE + 1] = 2;
		if (line && *line)
			return (line);
		free(line);
		return (NULL);
	}
	buffer_start[fd] = 0;
	buffer = buffer_start;
	return (1);
}

char	*get_next_line(int fd)
{
	static char	buffer_start[BUFFER_SIZE + 2];
	static char	*buffer = buffer_start;
	char		*line;
	int			i;
	int			len;

	if (buffer_start[BUFFER_SIZE + 1] == 2)
		return (NULL);
	line = malloc(sizeof(char));
	if (!line)
		return (0);
	len = 0;
	*line = 0;
	while (1)
	{
		if (*buffer == 0)
		{
			i = read(fd, buffer_start, BUFFER_SIZE);
			if (i < 0)
			{
				buffer_start[BUFFER_SIZE + 1] = 2;
				free(line);
				return (NULL);
			}
			else if (i == 0)
			{
				buffer_start[BUFFER_SIZE + 1] = 2;
				if (line && *line)
					return (line);
				free(line);
				return (NULL);
			}
			buffer_start[i] = 0;
			buffer = buffer_start;
		}
		i = 0;
		while (buffer[i] && buffer[i] != '\n')
			i++;
		if (buffer[i] == '\n')
		{
			append_to_str(&line, len, buffer, i + 1);
			buffer += i + 1;
			return (line);
		}
		append_to_str(&line, len, buffer, i);
		buffer += i;
		len += i;
	}
	return (NULL);
}

// int	main(void)
// {
// 	// int fd = open("gnlTester/files/multiple_nlx5", O_RDONLY);
// 	int fd = open("foo.txt", O_RDONLY);
// 	int i;

// 	for (i = 0; i < 20; i++)
// 	{
// 		char *line = get_next_line(fd);
// 		// printf("[%d]%s", i, line);
// 		printf("%s", line);
// 		free(line);
// 	}

// 	close(fd);
// }