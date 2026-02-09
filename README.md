*This project has been created as part of the 42 curriculum by danimend*

## Description
get_next_line has very straight goals; return line by line (delimited by \n) from a file per call of get_next_line(int fd), where fd is a FILE_DESCRIPTOR

## Instructions
```c
int	main(void)
{
	int fd = open("get_next_line.c", O_RDONLY);
	int i;

	for (i = 0; i < 20; i++)
	{
		char *line = get_next_line(fd);
		printf("%s", line);
		free(line);
	}

	close(fd);
    return (0);
}
```

## Resources

* man pages for `open` and `read`
* AI was used for some nuanced questions about the API eg `Does read append newline automatically?`

## Implementation
This implementation of get_next_line works by a rolling buffer. When we call get_next_line(), we fill in the buffer where:

1. A newline is found
    * Return string up to \n
    * Increment the buffer by strlen() up to \n
2. We reach the end of the buffer, but no newline
    * Return a copy of buffer with a \n
3. We reach EOF
    * Return a copy of buffer

The s_gnl struct maintains state across function calls using a static variable:
* `buffer_start` - Points to the static buffer array (size BUFFER_SIZE + 2)
* `buffer` - Moving pointer that tracks current position within buffer

This allows us to use a single static variable, but house multiple fields in struct for more flexibility.