#include "fcntl.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"

// typedef struct
// {
// 	char *buffer;
// 	char *original_ptr;
// } entry;

static int BUFFER_SIZE = 7;

int		ft_strlen(char *str)
{
	int	len;

	len	= 0;
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
	*str_ptr = temp;
	return (1);
}

int	read_buffer(char **buffer, int fd, int buffer_size, int read_count)
{
	static char *cache[4096];
	int rd_status;
	*buffer = cache[fd];

	rd_status = 1;
	if (ft_strlen(*buffer) == 0)
	{
		if (*buffer)
		{
			free(*buffer - BUFFER_SIZE);
		}
		*buffer = malloc(sizeof(char) * buffer_size);
		if (!*buffer)
			return -1;
		rd_status = read(fd, *buffer, buffer_size);
	}

	(*buffer) += read_count;
	cache[fd] = *buffer;
	return (rd_status);
}

char	*get_next_line(int fd)
{
	char *buffer;
	char *line;
	int i;

	line = malloc(0);
	i = 0;
	while (1)
	{
		i = 0;
		if (read_buffer(&buffer, fd, BUFFER_SIZE, 0) < 0)
			return (NULL);
		while (buffer[i] && buffer[i] != '\n')
			i++;

		if (buffer[i] == '\n')
		{
			str_append(&line, buffer, i + 1);
			read_buffer(&buffer, fd, BUFFER_SIZE, i + 1);
			return (line);
		}
		else if (!buffer[i])
		{
			str_append(&line, buffer, i);
			if (read_buffer(&buffer, fd, BUFFER_SIZE, i) <= 0)
				return (line);
		}
	}
	return (line);
}

int	main(void)
{
	int	fd_0 = open("foo.txt", O_RDONLY);

	for (int i = 0; i < 6; i++)
	{
		printf("line = %s", get_next_line(fd_0));
	}
	return (0);
}
