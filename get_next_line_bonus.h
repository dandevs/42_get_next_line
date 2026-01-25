#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

typedef struct s_line_reader
{
	char	*buffer;
	char	*buffer_start;
	int		fd;
	int		bytes_left;
	int		bytes_read;
}	t_line_reader;

#endif
