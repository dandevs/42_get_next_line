#include "get_next_line_bonus.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	str_append(char **str_ptr, char *to_append, int append_count)
{
	char	*str;
	char	*temp;
	int		i;
	int		j;

	str = *str_ptr;
	temp = malloc(sizeof(char) * (ft_strlen(str) + append_count + 1));
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
