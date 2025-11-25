#include "shell.h"

/**
 * split_line - splits a line into tokens
 * @line: input
 * Return: array of strings
 */
char **split_line(char *line)
{
char **tokens = malloc(sizeof(char *) * 64);
char *token;
int index = 0;

if (!tokens)
return (NULL);

token = strtok(line, " \t\r\n");
while (token)
{
tokens[index++] = token;
token = strtok(NULL, " \t\r\n");
}
tokens[index] = NULL;

return (tokens);
}
