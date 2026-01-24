
#include "fcntl.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"

static int BUFFER_SIZE = 7;

typedef struct s_line_reader
{
	char	*buffer;
	char	*buffer_start;
	int		fd;
	int		remaining_bytes;
} t_line_reader;

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
	free(str);
	*str_ptr = temp;
	return (1);
}

t_line_reader	*get_rdl_entry(int fd)
{
	static t_line_reader	*cache[4096];
	t_line_reader			*entry;

	entry = cache[fd];

	if (!entry)
	{
		entry = malloc(sizeof(t_line_reader));
		entry->fd = fd;
		entry->buffer = malloc(BUFFER_SIZE);
		entry->buffer_start = entry->buffer;
		entry->remaining_bytes = read(fd, entry->buffer, BUFFER_SIZE);
		cache[fd] = entry;
	}

	return (entry);
}

void	rdl_entry_consume(t_line_reader *entry, int count)
{
	if (count == 0)
	{
		if (entry->remaining_bytes == 0)
		{
			entry->buffer = entry->buffer_start;
			entry->remaining_bytes = read(entry->fd, entry->buffer, BUFFER_SIZE);
		}
	}
	else
	{
		entry->buffer += count;
		entry->remaining_bytes -= count;
	}
}

char	*read_line(int fd)
{
	t_line_reader	*entry;
	char		*line;
	int			i;

	line = malloc(1);
	entry = get_rdl_entry(fd);
	*line = 0;
	
	while (1)
	{
		i = 0;
		rdl_entry_consume(entry, 0);

		if (entry->remaining_bytes == 0)
			break ;

		while (i < entry->remaining_bytes && entry->buffer[i] != '\n')
			i++;

		if (entry->buffer[i] == '\n')
		{
			str_append(&line, entry->buffer, i + 1);
			rdl_entry_consume(entry, i + 1);
			return (line);
		}
		else 
		{
			str_append(&line, entry->buffer, i);
			rdl_entry_consume(entry, i);
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
	// printf("%s", read_line(fd_0));
	// printf("%s", read_line(fd_0));
	// printf("%s", read_line(fd_0));
	printf("\n----\n");
	return (0);
}