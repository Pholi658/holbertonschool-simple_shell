#include "shell.h"

/**
 * find_executable - searches for command in PATH
 * @cmd: command name
 * Return: full path or NULL if not found
 */
char *find_executable(char *cmd)
{
    char *path_env, *path_dup, *full_path;
    char *token;
    size_t len;

    if (!cmd)
        return (NULL);

    /* Absolute path given */
    if (cmd[0] == '/' || cmd[0] == '.')
    {
        if (access(cmd, X_OK) == 0)
            return (strdup(cmd));
        return (NULL);
    }

    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);

    path_dup = strdup(path_env);
    token = strtok(path_dup, ":");
    while (token)
    {
        len = strlen(token) + strlen(cmd) + 2;
        full_path = malloc(len);
        if (!full_path)
        {
            free(path_dup);
            return (NULL);
        }
        snprintf(full_path, len, "%s/%s", token, cmd);
        if (access(full_path, X_OK) == 0)
        {
            free(path_dup);
            return (full_path);
        }
        free(full_path);
        token = strtok(NULL, ":");
    }

    free(path_dup);
    return (NULL);
}

