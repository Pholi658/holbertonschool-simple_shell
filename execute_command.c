#include "shell.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * execute_command - Forks and executes a command with arguments
 * @args: NULL-terminated array of strings (argv)
 */
void execute_command(char **args)
{
    pid_t pid;
    int status;

    if (args[0] == NULL)
        return;

    pid = fork();
    if (pid == 0) /* child */
    {
        execve(args[0], args, environ);
        perror("./hsh"); /* execve failed */
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) /* parent */
        wait(&status);
    else
        perror("fork");
}
