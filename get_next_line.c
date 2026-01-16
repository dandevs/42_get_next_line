#include "fcntl.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"

void	iterate(char *buffer)
{
	char str[1024];
	int	i = 0;

	while (*buffer != '\n' && *buffer != 0)
	{
		str[i] = *buffer;
		buffer++;
		i++;
	}

	// printf("iteration: %s\n", str);
	write(0, str, i);
}

int	main(void)
{
	int	fd = open("foo.txt", O_RDONLY);
	void *buffer = malloc(1024);
	read(fd, buffer, 1024);

	// printf("%s", (char*)buffer);
	char *str = (char*)buffer;

	iterate(str);
	iterate(str);
	iterate(str);

	free(buffer);
	return (0);
}
