#include "shell.h"

/**
 * find_executable - find full path for a command
 * @cmd: command
 * Return: path string or NULL
 */
char *find_executable(char *cmd)
{
    char *path_env, *path_dup, *full_path;
    char *dir;
    size_t len;

    if (access(cmd, X_OK) == 0)
        return (_strdup(cmd));

    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);

    path_dup = _strdup(path_env);
    if (!path_dup)
        return (NULL);

    dir = strtok(path_dup, ":");
    while (dir)
    {
        len = _strlen(dir) + _strlen(cmd) + 2;
        full_path = malloc(sizeof(char) * len);
        if (!full_path)
        {
            free(path_dup);
            return (NULL);
        }
        _strcpy(full_path, dir);
        _strcat(full_path, "/");
        _strcat(full_path, cmd);

        if (access(full_path, X_OK) == 0)
        {
            free(path_dup);
            return (full_path);
        }

        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_dup);
    return (NULL);
}

