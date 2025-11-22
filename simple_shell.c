#include "shell.h"
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * trim_whitespace - Remove leading/trailing spaces and newlines
 * @str: input string
 *
 * Return: pointer to trimmed string
 */
char *trim_whitespace(char *str)
{
char *end;

if (str == NULL)
        return NULL;

 /* Trim leading space */
while (*str && isspace((unsigned char)*str))
str++;

if (*str == 0)  /* all spaces */
return str;

/* Trim trailing space */
end = str + strlen(str) - 1;
while (end > str && isspace((unsigned char)*end))
end--;

*(end + 1) = '\0';
return str;
}

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
/* Prompt only in interactive mode */
if (isatty(STDIN_FILENO))
printf("#cisfun$ ");

line = read_line();
if (line == NULL)  /* Ctrl+D or EOF */
{
if (isatty(STDIN_FILENO))
printf("\n");
break;
}

line = trim_whitespace(line);

if (strcmp(line, "exit") == 0)
{
free(line);
break;
}

if (*line != '\0')  /* skip empty lines */
execute_command(line);

free(line);
}

return 0;
}
