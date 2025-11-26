#include "shell.h"

/**
 * execute_command - executes a command in a child process
 * @args: array of arguments
 * @prog_name: name of shell program (argv[0])
 * @line_count: line counter for errors
 * Return: 0 on success, -1 on failure
 */
int execute_command(char **args, char *prog_name, unsigned int *line_count)
{
    pid_t pid;
    int status;
    char *cmd_path;

    if (!args || !args[0])
        return (0);

    /* Built-in exit */
    if (strcmp(args[0], "exit") == 0)
        exit(0);

    /* Find executable in PATH or absolute path */
    cmd_path = find_executable(args[0]);
    if (!cmd_path)
    {
        fprintf(stderr, "%s: %u: %s: not found\n", prog_name, *line_count, args[0]);
        return (-1);
    }

    pid = fork();
    if (pid == 0) /* child */
    {
        execve(cmd_path, args, environ);
        perror("execve");
        exit(1);
    }
    else if (pid > 0) /* parent */
        waitpid(pid, &status, 0);
    else
    {
        perror("fork");
        free(cmd_path);
        return (-1);
    }

    free(cmd_path);
    return (0);
}

