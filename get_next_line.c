#include "get_next_line.h"

static int	append_to_str(char **line, int line_len, char *app, int app_len)
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

static char	*refill_buffer(int fd, char (*b_s)[], char **b, char *line)
{
	int	rd;

	rd = read(fd, *b_s, BUFFER_SIZE);
	if (rd < 0)
	{
		(*b_s)[BUFFER_SIZE + 1] = 1;
		free(line);
		return (NULL);
	}
	else if (rd == 0)
	{
		(*b_s)[BUFFER_SIZE + 1] = 1;
		if (line && *line)
			return (line);
		free(line);
		return (NULL);
	}
	(*b_s)[rd] = 0;
	*b = *b_s;
	return ((void *)-1);
}

static char	*iterate_next_line_buffer(char **buffer, char **line, int *len)
{
	int	i;

	i = 0;
	while ((*buffer)[i] && (*buffer)[i] != '\n')
		i++;
	if ((*buffer)[i] == '\n')
	{
		append_to_str(line, *len, *buffer, i + 1);
		*buffer += i + 1;
		return (*line);
	}
	append_to_str(line, *len, *buffer, i);
	*buffer += i;
	*len += i;
	return ((void *)-1);
}

char	*get_next_line(int fd)
{
	static char	b_ptr[BUFFER_SIZE + 2];
	static char	*buffer = b_ptr;
	char		*line;
	char		*ret;
	int			len;

	if (BUFFER_SIZE < 0 || fd < 0 || fd >= 1024 || b_ptr[BUFFER_SIZE + 1] == 1)
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
			ret = refill_buffer(fd, &b_ptr, &buffer, line);
			if (ret != (void *)-1)
				return (ret);
		}
		if (iterate_next_line_buffer(&buffer, &line, &len) != (void *)-1)
			return (line);
	}
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