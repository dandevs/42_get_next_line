#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#define BUFFER_SIZE 8

unsigned int	*get_buffer()
{
	static unsigned int	buffer[BUFFER_SIZE + 3];
	return (buffer);
}

int	append_to_str(char **line, int line_len, char *to_append, int append_len)
{
	char	*str;
	int		i;
	int		j;

	str = malloc(sizeof(char) * (line_len + append_len + 1));
	if (!str)
		return (-1);
	i = 0;
	j = 0;
	while (i < line_len)
	{
		str[i] = (*line)[i];
		i++;
	}
	while (j < append_len)
		str[i++] = to_append[j++];
	str[i] = 0;
	free(*line);
	*line = str;
	return (i);
}

char	*get_next_line(int fd)
{
	static char	leftover[BUFFER_SIZE + 1];
	char		buffer[BUFFER_SIZE];
	char		*line;
	int			rd;
	int			i;
	int			len;

	rd = read(fd, buffer, BUFFER_SIZE);
	if (rd <= 0)
		return (NULL);
	line = malloc(sizeof(char));
	if (!line)
		return (NULL);
	*line = 0;

	i = 0;
	len = 0;
	while (leftover[i])
		i++;
	if (i)
		len = append_to_str(&line, 0, leftover, i);
	if (len < 0)
		return (NULL);

	while (1)
	{
		if (rd <= 0)
		{
			if (*line != 0)
				return (line);
			return (NULL);
		}
		i = 0;
		while (i < rd && buffer[i] != '\n')
			i++;
		if (buffer[i] == '\n')
		{
			len = append_to_str(&line, len, buffer, ++i);
			if (len >= 0)
			{
				int j = 0;
				while (j < (rd - i))
					leftover[j++] = buffer[i + j];
				leftover[j] = 0;
				return (line);
			}
			return (NULL);
		}
		else
		{
			len = append_to_str(&line, len, buffer, rd);
			if (len < 0)
				return (NULL);
		}
	}
	
	return (NULL);
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

	close(fd);
}