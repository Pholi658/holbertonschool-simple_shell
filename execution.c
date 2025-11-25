#include "shell.h"

/**
 * printerror - prints "not found" error for a command
 * @command: argv-style command array (command[0] is the program)
 *
 * Note: does NOT free any tokens allocated by strtok.
 */
void printerror(char *const command[])
{
	fprintf(stderr, "./hsh: 1: %s: not found\n", command[0]);
}

/**
 * _getenv - get environment variable value from envp
 * @name: name of environment variable (e.g., "PATH")
 * @envp: environment array
 *
 * Return: pointer to variable value (within envp string), or NULL
 */
char *_getenv(const char *name, char **envp)
{
	char **env, *sep;

	if (name == NULL || envp == NULL)
		return (NULL);

	for (env = envp; *env != NULL; env++)
	{
		sep = strchr(*env, '=');
		if (sep != NULL)
		{
			if ((size_t)(sep - *env) == strlen(name) &&
			    strncmp(*env, name, sep - *env) == 0)
				return (sep + 1);
		}
	}
	return (NULL);
}

/**
 * pathfinder - find full path for cmd by searching PATH
 * @cmd: command name (e.g., "ls")
 * @command: argv array (will be updated to point to fullpath if found)
 * @envp: environment array
 *
 * Return: command (argv) with command[0] replaced by malloc'ed fullpath if
 *         found, or NULL if not found. If cmd already contains a '/', the
 *         function tests the path and returns command if accessible.
 *
 * Caller must free(command[0]) if it was replaced (i.e., pointer differs from
 * original token).
 */
char **pathfinder(char *cmd, char **command, char **envp)
{
	char *path_env, *path_dup, *dir;
	char *fullpath = NULL;
	size_t len;

	if (cmd == NULL || command == NULL)
		return (NULL);

	/* If cmd contains a slash, treat it as a path */
	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
		{
			/* keep command[0] as-is */
			return (command);
		}
		return (NULL);
	}

	path_env = _getenv("PATH", envp);
	if (path_env == NULL)
		return (NULL);

	path_dup = strdup(path_env);
	if (path_dup == NULL)
		return (NULL);

	dir = strtok(path_dup, ":");
	while (dir != NULL)
	{
		len = strlen(dir) + strlen(cmd) + 2;
		fullpath = malloc(len);
		if (fullpath == NULL)
		{
			free(path_dup);
			return (NULL);
		}
		snprintf(fullpath, len, "%s/%s", dir, cmd);

		if (access(fullpath, X_OK) == 0)
		{
			/* Replace command[0] with malloc'ed fullpath */
			command[0] = fullpath;
			free(path_dup);
			return (command);
		}
		free(fullpath);
		fullpath = NULL;
		dir = strtok(NULL, ":");
	}

	free(path_dup);
	return (NULL);
}

/**
 * execute - fork and execute a command if found in PATH (or absolute path)
 * @command: argv-style array (NULL-terminated)
 * @envp: environment array
 *
 * Return: 0 on success (command forked/executed), -1 on error (command not found)
 *
 * Behavior:
 *  - If command not found: prints error and returns -1 (no fork).
 *  - If found: forks and execve; parent waits for child.
 *  - Caller is responsible for freeing tokens and the original input buffer.
 */
int execute(char *command[], char **envp)
{
	pid_t id;
	int status;
	char **temp;
	char *orig0 = NULL;

	if (command == NULL || command[0] == NULL)
		return (-1);

	/* Save original pointer to command[0] so we can detect allocation */
	orig0 = command[0];

	/* Find executable path; pathfinder may replace command[0] */
	temp = pathfinder(command[0], (char **)command, envp);

	if (temp == NULL)
	{
		/* Not found — print error, do not fork */
		printerror(command);
		return (-1);
	}

	id = fork();
	if (id < 0)
	{
		perror("fork failed");
		/* If pathfinder allocated a new string, free it */
		if (command[0] != orig0)
			free(command[0]);
		return (-1);
	}
	else if (id == 0)
	{
		/* Child */
		execve(command[0], command, envp);
		/* If execve returns, it's an error */
		perror("execve");
		_exit(EXIT_FAILURE);
	}
	/* Parent */
	wait(&status);

	/* If pathfinder returned an allocated fullpath (different pointer), free it */
	if (command[0] != orig0)
	{
		free(command[0]);
		command[0] = orig0;
	}

	(void)status;
	return (0);
}
