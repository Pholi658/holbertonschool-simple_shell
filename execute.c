/* execute.c */
#include "shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>

/**
 * execute_command - find & execute a command (uses execve)
 * @args: NULL-terminated argument vector (argv-style)
 * @line_count: pointer to current line number (for error messages); may be unused
 *
 * Return: 0 on success (command executed), -1 on failure (command not found or error)
 */
int execute_command(char **args, unsigned int *line_count)
{
    pid_t pid;
    int status = 0;
    char *path;

    (void)line_count; /* suppress unused-parameter warning */

    if (!args || !args[0])
        return (0);

    /* builtin: exit */
    if (_strcmp(args[0], "exit") == 0)
    {
        free_args(args);
        exit(0);
    }

    /* Find the full path (handles absolute/relative paths and PATH search) */
    path = find_executable(args[0]);
    if (!path)
    {
        /* print error like /bin/sh: prog: lineno: cmd: not found if you prefer,
           but simple message below is acceptable for debugging */
        fprintf(stderr, "%s: %s: not found\n", args[0], ""); /* keep brief */
        return (-1);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(path);
        return (-1);
    }

    if (pid == 0) /* child */
    {
        if (execve(path, args, environ) == -1)
        {
            perror("execve");
            free(path);
            _exit(127);
        }
    }
    else /* parent */
    {
        while (waitpid(pid, &status, 0) == -1)
        {
            if (errno == EINTR)
                continue;
            perror("waitpid");
            break;
        }
    }

    free(path);
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    return (status);
}

