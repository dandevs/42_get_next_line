#include "get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd1;
	int		fd2;
	char	*line;

	fd1 = open("test_file.txt", O_RDONLY);
	if (fd1 < 0)
	{
		printf("Error opening file\n");
		return (1);
	}

	printf("=== Reading from fd1 ===\n");
	while ((line = read_line(fd1)) != NULL)
	{
		printf("%s", line);
		free(line);
	}

	close(fd1);
	return (0);
}
