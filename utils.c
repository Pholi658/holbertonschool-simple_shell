#include "shell.h"

void free_args(char **args)
{
    int i = 0;
    if (!args)
        return;
    while (args[i])
        free(args[i++]);
    free(args);
}

char **split_line(char *line)
{
    char **tokens = NULL;
    char *token;
    int count = 0;
    char *line_dup;

    if (!line)
        return (NULL);

    line_dup = strdup(line);
    token = strtok(line_dup, " \t\n");
    while (token)
    {
        tokens = realloc(tokens, sizeof(char *) * (count + 2));
        tokens[count] = strdup(token);
        count++;
        token = strtok(NULL, " \t\n");
    }
    if (tokens)
        tokens[count] = NULL;
    free(line_dup);
    return (tokens);
}

char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    if (getline(&line, &bufsize, stdin) == -1)
    {
        free(line);
        return (NULL);
    }
    return (line);
}

void print_prompt(void)
{
    write(STDOUT_FILENO, "($) ", 4);
}

