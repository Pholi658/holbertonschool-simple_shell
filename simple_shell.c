#include "shell.h"

/**
 * main - Entry point of the simple shell
 *
 * Return: Always 0
 */
int main(void)
{
char *line;

while (1)
{
printf("#cisfun$ ");
line = read_line();

if (line == NULL) /* Ctrl+D */
{
printf("\n");
break;
}

if (strcmp(line, "exit") == 0)
{
free(line);
break;
}

execute_command(line);
free(line);
}

return (0);
}
