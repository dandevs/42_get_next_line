/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:45:42 by danimend          #+#    #+#             */
/*   Updated: 2026/02/10 13:06:37 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*try_get_next_line(int fd)
{
	t_gnl		*gnl;
	char		*line;
	char		*ret;
	int			len;

	if (BUFFER_SIZE < 0 || fd < 0 || fd >= 512)
		return (NULL);
	gnl = get_gnl();
	line = malloc(sizeof(char));
	if (!line)
		return (0);
	len = 0;
	*line = 0;
	while (1)
	{
		if (*gnl->buffer == 0)
		{
			ret = refill_buffer(fd, gnl, line);
			if (ret != (void *)-1)
				return (ret);
		}
		if (iterate_next_line_buffer(gnl, &line, &len) != (void *)-1)
			return (line);
	}
}

char	*get_next_line(int fd)
{
	char	*line;

	line = try_get_next_line(fd);
	if (line == NULL)
	{
		get_gnl()->buffer_start[0] = 0;
		return (NULL);
	}
	else
		return (line);
}

// int	main(void)
// {
// 	// int fd = open("gnlTester/files/multiple_nlx5", O_RDONLY);
// 	int fd = open("get_next_line.c", O_RDONLY);
// 	int i;

// 	for (i = 0; i < 10; i++)
// 	{
// 		char *line = get_next_line(fd);
// 		printf("%s", line);
// 		free(line);
// 	}

// 	close(fd);
// }