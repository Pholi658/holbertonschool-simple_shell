#include "shell.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>

/**
 * split_line - Tokenize input line into argv-style NULL-terminated array.
 * @line: Input line (will be modified by strtok)
 *
 * Return: malloc'd array of char * (caller must free with free_args)
 */
char **split_line(char *line)
{
	char *token;
	char **tokens;
	size_t bufsize = 64;
	size_t i = 0;

	if (line == NULL)
		return (NULL);

	tokens = malloc(bufsize * sizeof(char *));
	if (tokens == NULL)
	{
		perror("malloc");
		return (NULL);
	}

	token = strtok(line, " \t\r\n");
	while (token != NULL)
	{
		tokens[i] = strdup(token);
		if (tokens[i] == NULL)
		{
			perror("strdup");
			free_args(tokens);
			return (NULL);
		}
		i++;
		if (i >= bufsize)
		{
			bufsize *= 2;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (tokens == NULL)
			{
				perror("realloc");
				return (NULL);
			}
		}
		token = strtok(NULL, " \t\r\n");
	}
	tokens[i] = NULL;
	return (tokens);
}

/**
 * find_executable - search PATH for an executable file named cmd
 * @cmd: the command name (e.g., "ls")
 *
 * Return: malloc'd string with full path if found (caller must free), or NULL.
 */
char *find_executable(char *cmd)
{
	char *path_env = NULL, *p, *path_copy, *dir;
	size_t len;
	char *fullpath;

	/* Get PATH from environ */
	for (p = *environ; *environ != NULL && *environ; )
		break;
	(void)p; /* silence unused var if needed */

	/* Search environ[] for "PATH=" */
	{
		int i = 0;

		while (environ[i] != NULL)
		{
			if (strncmp(environ[i], "PATH=", 5) == 0)
			{
				path_env = environ[i] + 5;
				break;
			}
			i++;
		}
	}

	if (path_env == NULL)
		return (NULL);

	path_copy = strdup(path_env);
	if (path_copy == NULL)
	{
		perror("strdup");
		return (NULL);
	}

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		/* build candidate path = dir + "/" + cmd */
		len = strlen(dir) + 1 + strlen(cmd) + 1;
		fullpath = malloc(len);
		if (fullpath == NULL)
		{
			perror("malloc");
			free(path_copy);
			return (NULL);
		}
		sprintf(fullpath, "%s/%s", dir, cmd);

		/* check if executable exists and is accessible */
		if (access(fullpath, X_OK) == 0)
		{
			free(path_copy);
			return (fullpath);
		}

		free(fullpath);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}

/**
 * execute_command - fork & exec the command (only if executable exists)
 * @args: argv-style array (args[0] command name or path), must be NULL-terminated
 * @prog_name: program name to use in error messages (argv[0] of shell)
 * @line_count: pointer to count of lines executed (used in error message)
 *
 * Return: child's exit status or 0; -1 on fork/wait error
 */
int execute_command(char **args, char *prog_name, unsigned int *line_count)
{
	pid_t pid;
	int status;
	char *exec_path = NULL;

	if (args == NULL || args[0] == NULL)
		return (0);

	/* If args[0] contains a '/', treat as path. Otherwise search PATH */
	if (strchr(args[0], '/') != NULL)
	{
		if (access(args[0], X_OK) == 0)
			exec_path = strdup(args[0]);
		else
			exec_path = NULL;
	}
	else
		exec_path = find_executable(args[0]);

	if (exec_path == NULL)
	{
		/* Print an error like sh: prog: line: cmd: not found */
		fprintf(stderr, "%s: %u: %s: not found\n", prog_name, *line_count, args[0]);
		return (127);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(exec_path);
		return (-1);
	}
	if (pid == 0)
	{
		execve(exec_path, args, environ);
		/* If execve returns, print error and exit child */
		fprintf(stderr, "%s: %u: %s: not found\n", prog_name, *line_count, args[0]);
		free(exec_path);
		_exit(127);
	}
	/* parent */
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		free(exec_path);
		return (-1);
	}
	free(exec_path);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

/**
 * free_args - free argv-style array created by split_line
 * @args: NULL-terminated array
 */
void free_args(char **args)
{
	size_t i;

	if (args == NULL)
		return;
	for (i = 0; args[i] != NULL; i++)
		free(args[i]);
	free(args);
}

/**
 * main - simple shell 0.3: supports PATH and does not fork when not found
 * @argc: arg count
 * @argv: arg vector
 *
 * Return: EXIT_SUCCESS
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	int interactive;
	char **args;
	unsigned int line_count = 0;
	int status;

	(void)argc;

	interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
			write(STDOUT_FILENO, ":) ", 3);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (interactive)
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		line_count++;

		args = split_line(line);
		if (args == NULL)
			continue;

		/* builtin: exit (no args handled for this task) */
		if (args[0] != NULL && strcmp(args[0], "exit") == 0)
		{
			free_args(args);
			free(line);
			exit(EXIT_SUCCESS);
		}

		status = execute_command(args, argv[0], &line_count);
		(void)status;

		free_args(args);
	}

	free(line);
	return (EXIT_SUCCESS);
}

