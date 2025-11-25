#include "shell.h"

/**
 * main - Simple shell main loop
 * Return: 0
 */
int main(void)
{
char *line = NULL;
char **args = NULL;

while (1)
{
printf("#cisfun$ ");

line = read_line();
if (!line)
{
printf("\n");
break;
}

args = split_line(line);

if (args[0] != NULL)
{
/* built-in exit */
if (strcmp(args[0], "exit") == 0)
{
free(line);
free(args);
break;
}

execute_command(args);
}

free(line);
free(args);
}

return (0);
}
