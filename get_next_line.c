#include "get_next_line_bonus.h"
#include "stdlib.h"
#include "unistd.h"

char	*get_next_line(int fd)
{
	t_line_reader	*reader;
	char			*line;

	if (fd < 0 || fd >= 4096 || BUFFER_SIZE <= 0)
		return (NULL);
	line = malloc(sizeof(char));
	if (!line)
		return (NULL);
	reader = get_line_reader(fd);
	if (!reader)
		return (NULL);
	*line = 0;
	if (line_reader_read(reader, &line))
		return (line);
	else
	{
		free(reader);
		free(line);
		return (NULL);
	}
}

/*
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
*/
