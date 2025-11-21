#include "shell.h"

/**
 * execute_command - Forks and runs a command using execve
 * @line: command to run
 *
 * Return: nothing
 */
void execute_command(char *line)
{
pid_t pid;
char *args[2];

pid = fork();
if (pid == -1)
{
perror("fork");
return;
}

if (pid == 0)
{
args[0] = line;
args[1] = NULL;

execve(line, args, environ);
perror(line);
exit(EXIT_FAILURE);
}
else
{
wait(NULL);
}
}
