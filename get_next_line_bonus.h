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
	char	*buffer_start;
	char	*buffer;
}	t_gnl;

typedef struct s_gnl_cache
{
	int		initialized;
	char	buffer[BUFFER_SIZE + 1];
	t_gnl	gnl;
}	t_gnl_cache;

t_gnl	*get_gnl(int fd);
char	*get_next_line(int fd);
int		append_to_str(char **line, int line_len, char *app, int app_len);
char	*refill_buffer(int fd, t_gnl *gnl, char *line);
char	*iterate_next_line_buffer(t_gnl *gnl, char **line, int *len);

#endif
