#include "shell.h"

/**
 * printerror - prints error when command not found
 * @command: command array
 */
void printerror(char *const command[])
{
    fprintf(stderr, "./hsh: 1: %s: not found\n", command[0]);
}

/**
 * _getenv - gets environment variable value
 * @name: variable name
 * @envp: environment array
 * Return: pointer to value or NULL
 */
char *_getenv(const char *name, char **envp)
{
    char **env, *sep;

    for (env = envp; *env != NULL; env++)
    {
        sep = strchr(*env, '=');
        if (sep != NULL && strncmp(*env, name, sep - *env) == 0)
            return sep + 1;
    }
    return NULL;
}

/**
 * pathfinder - finds the full path of a command
 * @cmd: command name
 * @command: original command array
 * @envp: environment
 * Return: updated command array or NULL
 */
char **pathfinder(char *cmd, char **command, char **envp)
{
    char *path_env, *path_dup, *dir_tok, *fullpath;
    size_t len;

    if (strchr(cmd, '/') != NULL && access(cmd, F_OK) == 0)
    {
        command[0] = cmd;
        return command;
    }

    path_env = _getenv("PATH", envp);
    if (!path_env)
        return NULL;

    path_dup = strdup(path_env);
    if (!path_dup)
        return NULL;

    dir_tok = strtok(path_dup, ":");
    while (dir_tok)
    {
        len = strlen(dir_tok) + strlen(cmd) + 2;
        fullpath = malloc(len);
        if (!fullpath)
        {
            free(path_dup);
            return NULL;
        }
        sprintf(fullpath, "%s/%s", dir_tok, cmd);
        if (access(fullpath, F_OK) == 0)
        {
            command[0] = fullpath;
            free(path_dup);
            return command;
        }
        free(fullpath);
        dir_tok = strtok(NULL, ":");
    }
    free(path_dup);
    return NULL;
}

/**
 * execute - executes a command array
 * @command: array of command/arguments
 * @envp: environment
 * Return: child exit status or 127 if command not found
 */
int execute(char *command[], char **envp)
{
    pid_t id;
    int status;
    char **temp;

    if (!command || !command[0])
        return -1;

    temp = pathfinder(command[0], command, envp);
    if (!temp)
    {
        printerror(command);
        return 127;
    }

    id = fork();
    if (id < 0)
    {
        perror("fork failed");
        return -1;
    }
    else if (id == 0)
    {
        /* Child process */
        execve(command[0], command, envp);
        /* If execve fails */
        printerror(command);
        _exit(127);
    }

    /* Parent process waits */
    wait(&status);

    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    return -1;
}
