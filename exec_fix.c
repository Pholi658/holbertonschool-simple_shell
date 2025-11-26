/* exec_fix.c
 * Safe command execution helper for simple_shell.
 * - Unique function name hsh_execute_command to avoid symbol collisions.
 * - Uses PATH search and direct path checking (commands with '/').
 * - Does NOT fork if command cannot be found (returns 127).
 * - Uses only allowed functions: access, malloc, free, execve, fork, waitpid, perror, fprintf, string.h functions.
 */

#include "shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

static char *strdup_like(const char *s)
{
    size_t len;
    char *p;

    if (!s)
        return NULL;
    len = strlen(s);
    p = malloc(len + 1);
    if (!p)
        return NULL;
    memcpy(p, s, len + 1);
    return p;
}

/* Return PATH value pointer from env (not duplicated) or NULL */
static char *get_path_env(char **env)
{
    int i;

    if (!env)
        return NULL;
    for (i = 0; env[i]; i++)
    {
        if (strncmp(env[i], "PATH=", 5) == 0)
            return env[i] + 5;
    }
    return NULL;
}

/* Search PATH for cmd or if cmd contains '/', test it directly.
 * Returns malloc'd string with full path (caller must free), or NULL if not found.
 */
static char *find_in_path(char *cmd, char **env)
{
    char *path_env, *path_copy, *token, *candidate;
    size_t needed;

    if (!cmd)
        return NULL;

    /* If command contains '/', treat as path */
    if (strchr(cmd, '/') != NULL)
    {
        if (access(cmd, X_OK) == 0)
            return strdup_like(cmd);
        return NULL;
    }

    path_env = get_path_env(env);
    if (!path_env)
        return NULL;

    path_copy = strdup_like(path_env);
    if (!path_copy)
        return NULL;

    token = strtok(path_copy, ":");
    while (token)
    {
        needed = strlen(token) + 1 + strlen(cmd) + 1;
        candidate = malloc(needed);
        if (!candidate)
        {
            free(path_copy);
            return NULL;
        }
        strcpy(candidate, token);
        strcat(candidate, "/");
        strcat(candidate, cmd);

        if (access(candidate, X_OK) == 0)
        {
            free(path_copy);
            return candidate; /* caller frees */
        }
        free(candidate);
        token = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

/* Public function used by the shell.
 * - args: NULL-terminated argv array (args[0] is command)
 * - env: environment pointer (use environ or pass envp)
 * - prog_name: argv[0] of shell (for error messages)
 * - line_count: current input line number (for error message formatting)
 *
 * Returns:
 * - 127 if command not found (no fork performed)
 * - child's exit status on success
 * - other positive numbers on errors
 */
int hsh_execute_command(char **args, char **env, char *prog_name, unsigned int line_count)
{
    pid_t pid;
    int status = 0;
    char *path;

    if (!args || !args[0])
        return 0;

    path = find_in_path(args[0], env);
    if (!path)
    {
        /* Command not found — print same style as /bin/sh: prog: lineno: cmd: not found */
        if (prog_name == NULL)
            prog_name = "hsh";
        fprintf(stderr, "%s: %u: %s: not found\n", prog_name, line_count, args[0]);
        return 127;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(path);
        return 1;
    }

    if (pid == 0)
    {
        /* child */
        if (execve(path, args, env) == -1)
        {
            perror("execve");
            free(path);
            _exit(1);
        }
    }
    else
    {
        /* parent */
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
        return WEXITSTATUS(status);
    if (WIFSIGNALED(status))
        return 128 + WTERMSIG(status);
    return status;
}

