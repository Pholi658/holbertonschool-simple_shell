#include "shell.h"

/**
 * read_line - Reads a line from stdin
 * Return: pointer to buffer
 */
char *read_line(void)
{
char *buffer = NULL;
size_t size = 0;

if (getline(&buffer, &size, stdin) == -1)
{
free(buffer);
return (NULL);
}

return (buffer);
}
