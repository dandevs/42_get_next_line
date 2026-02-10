/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:45:24 by danimend          #+#    #+#             */
/*   Updated: 2026/02/10 13:06:36 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*try_get_next_line(int fd)
{
	t_gnl	*gnl;
	char	*line;
	char	*ret;
	int		len;

	gnl = get_gnl(fd);
	if (!gnl)
		return (NULL);
	line = malloc(sizeof(char));
	if (!line)
		return (NULL);
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
	t_gnl	*gnl;

	if (BUFFER_SIZE < 0 || fd < 0 || fd >= 512)
		return (NULL);
	line = try_get_next_line(fd);
	if (line == NULL)
	{
		gnl = get_gnl(fd);
		if (gnl)
			gnl->buffer_start[0] = 0;
		return (NULL);
	}
	return (line);
}

// int	main(void)
// {
// 	int fd = open("get_next_line.c", O_RDONLY);
// 	int i;

// 	for (i = 0; i < 1500; i++)
// 	{
// 		char *line = get_next_line(fd);
// 		printf("%s", line);
// 		free(line);
// 	}

// 	close(fd);
//     return (0);
// }