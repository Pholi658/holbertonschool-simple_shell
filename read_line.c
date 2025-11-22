#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * read_line - Read a line of input from stdin
 * Return: pointer to the line (malloc'ed)
 */
char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;

    if (getline(&line, &bufsize, stdin) == -1)
    {
        free(line);
        return NULL;
    }

    return line;
}
