#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>

typedef struct s_line_reader
{
	char	*buffer;
	char	*buffer_start;
	int		fd;
	int		bytes_left;
	int		bytes_read;
}	t_line_reader;

int				str_append(char **s_p, int s_l, char *a, int ml);
t_line_reader	*get_line_reader(int fd);
t_line_reader	*line_reader_consume(t_line_reader *reader, int count);
int				line_reader_read(t_line_reader *reader, char **line);
char			*get_next_line(int fd);

#endif
