#include "shell.h"

/**
 * execute_command - runs a command
 * @args: argument vector
 */
void execute_command(char **args)
{
pid_t pid;
char *cmd_path;

cmd_path = find_command(args[0]);
if (!cmd_path)
{
fprintf(stderr, "%s: command not found\n", args[0]);
return;
}

pid = fork();
if (pid == -1)
{
perror("fork");
return;
}

if (pid == 0)
{
if (execve(cmd_path, args, environ) == -1)
{
perror("execve");
exit(EXIT_FAILURE);
}
}
else
{
waitpid(pid, NULL, 0);
}

if (cmd_path != args[0])
free(cmd_path);
}
