#include "shell.h"

/**
 * split_line - splits a line into words
 * @line: input line
 * Return: array of strings
 */
char **split_line(char *line)
{
    int bufsize = 64, pos = 0;
    char **tokens;
    char *token;

    tokens = malloc(sizeof(char *) * bufsize);
    if (!tokens)
        exit(1);

    token = strtok(line, " \t\n");
    while (token)
    {
        tokens[pos++] = _strdup(token);

        if (pos >= bufsize)
        {
            bufsize += 64;
            tokens = realloc(tokens, sizeof(char *) * bufsize);
            if (!tokens)
                exit(1);
        }

        token = strtok(NULL, " \t\n");
    }
    tokens[pos] = NULL;
    return (tokens);
}

/**
 * free_args - frees array of strings
 * @args: array of strings
 */
void free_args(char **args)
{
    int i = 0;

    if (!args)
        return;

    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}

