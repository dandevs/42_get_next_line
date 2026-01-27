#include "get_next_line_bonus.h"

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
	int			rd;

	buffer_ptr = cache[fd];
	if (!buffer_ptr)
	{
		buffer_ptr = malloc(sizeof(char **) * 2);
		buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buffer_ptr || !buffer)
			return (multi_free(buffer_ptr, buffer));
		rd = read(fd, buffer, BUFFER_SIZE);
		if (rd <= 0)
			return (multi_free(buffer_ptr, buffer));
		buffer[rd] = 0;
		buffer_ptr[0] = buffer;
		buffer_ptr[1] = buffer;
		cache[fd] = buffer_ptr;
	}
	if (!*buffer_ptr)
		return (NULL);
	return (buffer_ptr);
}

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	str_append(char **str_ptr, int str_len, char *to_append, int append_count)
{
	char	*str;
	char	*temp;
	int		i;
	int		j;

	str = *str_ptr;
	temp = malloc(sizeof(char) * str_len + append_count + 1);
	i = 0;
	if (!temp)
		return (0);
	while (str[i])
	{
		temp[i] = str[i];
		i++;
	}
	j = 0;
	while (j < append_count && to_append[j])
	{
		temp[i + j] = to_append[j];
		j++;
	}
	temp[i + j] = 0;
	free(str);
	*str_ptr = temp;
	return (1);
}
