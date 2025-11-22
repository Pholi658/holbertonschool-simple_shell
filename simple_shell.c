#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * main - Entry point for simple_shell
 *
 * Return: Always 0
 */
int main(void)
{
char *line;
char *cmd;
size_t start, end;

while (1)
{
if (isatty(STDIN_FILENO))
printf("#cisfun$ ");

line = read_line();
if (line == NULL) /* Ctrl+D or EOF */
{
if (isatty(STDIN_FILENO))
printf("\n");
break;
}

/* Trim leading spaces */
start = 0;
while (line[start] == ' ' || line[start] == '\t' || line[start] == '\n')
start++;

/* Trim trailing spaces */
end = strlen(line);
while (end > start && (line[end - 1] == ' ' || line[end - 1] == '\t' || line[end - 1] == '\n'))
end--;

line[end] = '\0';
cmd = line + start;

if (strcmp(cmd, "exit") == 0)
{
free(line);
break;
}

if (*cmd != '\0') /* skip empty lines */
execute_command(cmd);

free(line);
}

return (0);
}
