#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 5
#endif

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

typedef struct s_line_reader
{
	char	*buffer;
	char	*buffer_start;
	int		fd;
	int		bytes_left;
	int		bytes_read;
}	t_line_reader;

t_line_reader	*get_line_reader(int fd);
t_line_reader	*line_reader_consume(t_line_reader *reader, int count);
int				line_reader_free(t_line_reader **reader);
int				line_reader_read(t_line_reader *reader, char **line);
char			*get_next_line(int fd);
int				str_append(char **s_p, char *a, int ml);
int				ft_strlen(char *str);

#endif
