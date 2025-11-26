#include "shell.h"

char *find_executable(char *cmd)
{
    char *path_env;
    char *path_dup;
    char *dir;
    char *full_path;

    if (!cmd)
        return (NULL);

    if (cmd[0] == '/' || cmd[0] == '.')
    {
        if (access(cmd, X_OK) == 0)
            return (_strdup(cmd));
        return (NULL);
    }

    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);

    path_dup = _strdup(path_env);
    dir = strtok(path_dup, ":");
    while (dir)
    {
        int len;
        full_path = NULL;

        len = _strlen(dir) + 1 + _strlen(cmd) + 1;
        full_path = malloc(len);
        if (!full_path)
            break;
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

