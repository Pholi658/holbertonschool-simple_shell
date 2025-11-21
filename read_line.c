#include "shell.h"

/**
 * read_line - Reads a line from standard input
 *
 * Return: pointer to the line read, or NULL on EOF
 */
char *read_line(void)
{
char *line = NULL;
size_t size = 0;

if (getline(&line, &size, stdin) == -1)
{
free(line);
return (NULL);
}

line[strcspn(line, "\n")] = '\0';
return (line);
}
