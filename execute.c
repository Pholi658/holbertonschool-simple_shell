#include "shell.h"

/**
 * execute_command - execute command using fork/execve
 * @args: array of args
 * @line_count: current line number
 * Return: 0 always
 */
int execute_command(char **args, unsigned int *line_count)
{
    pid_t pid;
    int status;
    char *prog_path;

    if (_strcmp(args[0], "exit") == 0)
        exit(0);

    prog_path = find_executable(args[0]);
    if (!prog_path)
    {
        fprintf(stderr, "%s: %u: %s: not found\n",
                args[0], *line_count, args[0]);
        return (0);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(prog_path);
        return (1);
    }
    if (pid == 0)
    {
        execve(prog_path, args, environ);
        perror("execve");
        exit(1);
    }
    else
        wait(&status);

    free(prog_path);
    return (0);
}

