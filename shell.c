#include "shell.h"

/**
 * read_line - read a line from stdin using getline
 * Return: malloc'd string (without newline) or NULL on EOF/error
 */
char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t nread;

    nread = getline(&line, &bufsize, stdin);
    if (nread == -1)
    {
        free(line);
        return (NULL);
    }

    /* strip trailing newline if present */
    if (nread > 0 && line[nread - 1] == '\n')
        line[nread - 1] = '\0';

    return (line);
}
