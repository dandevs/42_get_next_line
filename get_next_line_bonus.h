#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
#include <fcntl.h>

char			*get_next_line(int fd);
int				str_append(char **s_p, int len, char *a, int ml);
int				ft_strlen(char *str);
void			*multi_free(void *ptr_0, void *ptr_1);
char			**get_buffer_ptr(int fd);

#endif
