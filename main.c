#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * main - entry point for the simple shell
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char **argv)
{
	char *line;
	char **args;
	size_t n;
	ssize_t read;
	unsigned int line_count = 0;

	(void)argc;
	(void)argv;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);

		line = NULL;
		n = 0;
		read = getline(&line, &n, stdin);

		if (read == -1)
		{
			write(STDOUT_FILENO, "\n", 1);
			free(line);
			exit(EXIT_SUCCESS);
		}

		line_count++;

		args = split_line(line);
		if (args[0] != NULL)
		{
			if (execute_command(args, &line_count) == -1)
			{
				free_args(args);
				free(line);
				continue;
			}
		}

		free_args(args);
		free(line);
	}

	return (0);
}

