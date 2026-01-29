#include "get_next_line.h"

char	***get_cache()
{
	static char **cache[1024];
	return (cache);
}

void	*multi_free(void *ptr_0, void *ptr_1)
{
	free(ptr_0);
	free(ptr_1);
	return (NULL);
}

char	*get_buffer_ptr(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	int			rd;

	if (buffer[BUFFER_SIZE] == 1)
		return (NULL);
}

static int	ft_strlen(char *str)
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
