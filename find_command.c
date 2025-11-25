#include "shell.h"

/**
 * find_command - search for a command in PATH
 * @cmd: command name
 * Return: full path or NULL
 */
char *find_command(char *cmd)
{
char *path, *path_copy, *dir;
char full_path[1024];
struct stat st;

if (strchr(cmd, '/'))
{
if (stat(cmd, &st) == 0)
return (cmd);
return (NULL);
}

path = getenv("PATH");
if (!path)
return (NULL);

path_copy = strdup(path);
if (!path_copy)
return (NULL);

dir = strtok(path_copy, ":");
while (dir)
{
snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);

if (stat(full_path, &st) == 0)
{
free(path_copy);
return strdup(full_path);
}

dir = strtok(NULL, ":");
}

free(path_copy);
return (NULL);
}
