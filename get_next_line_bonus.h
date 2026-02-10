/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danimend <danimend@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:45:31 by danimend          #+#    #+#             */
/*   Updated: 2026/02/10 12:45:32 by danimend         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <unistd.h>

typedef struct s_gnl
{
	char	buffer_start[BUFFER_SIZE + 1];
	char	*buffer;
}	t_gnl;

t_gnl	*get_gnl(int fd);
char	*get_next_line(int fd);
int		append_to_str(char **line, int line_len, char *app, int app_len);
char	*refill_buffer(int fd, t_gnl *gnl, char *line);
char	*iterate_next_line_buffer(t_gnl *gnl, char **line, int *len);

#endif
