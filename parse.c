#include "shell.h"

/**
 * split_line - splits a line into tokens
 * @line: input line
 * Return: NULL-terminated array of strings
 */
char **split_line(char *line)
{
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens)
        exit(EXIT_FAILURE);

    token = strtok(line, " \t\r\n");
    while (token != NULL)
    {
        tokens[position++] = _strdup(token);

        if (position >= bufsize)
        {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
                exit(EXIT_FAILURE);
        }

        token = strtok(NULL, " \t\r\n");
    }
    tokens[position] = NULL;
    return (tokens);
}

