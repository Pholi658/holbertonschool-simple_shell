#include "shell.h"

char **env = NULL;
char *command = NULL;

/**
 * print_env - prints the environment variables to stdout
 *
 * Return: 0 on success, -1 if env is NULL
 */
int print_env(void)
{
	int i = 0;

	if (env == NULL)
		return (-1);

	while (env[i])
	{
		printf("%s\n", env[i++]);
	}
	return (0);
}

/**
 * parse - tokenizes the input command and calls execute
 * @command: command buffer (modified by strtok)
 * @envp: environment array
 */
void parse(char command[], char **envp)
{
    char *arguments[11];
    char *token = NULL;
    int arg_count = 0;

    if (command == NULL)
        return;

    token = strtok(command, " ");
    while (token != NULL && arg_count < 10)
    {
        arguments[arg_count++] = token;
        token = strtok(NULL, " ");
    }

    if (arg_count >= 10)
    {
        fprintf(stderr, "Error: Too many arguments\n");
        return;
    }

    arguments[arg_count] = NULL;

    if (arg_count > 0)
    {
        if (envp != NULL)
            execute(arguments, envp);   /* <-- FIXED HERE */
        else
            fprintf(stderr, "./hsh: 1: %s: not found\n", arguments[0]);
    }
}


/**
 * input - read a line from stdin, handle EOF and strip newline
 * @command: pointer to buffer (may be reallocated by getline)
 * @size: pointer to buffer size
 */
void input(char **command, size_t *size)
{
	ssize_t read_bytes;

	read_bytes = getline(command, size, stdin);
	if (read_bytes == -1)
	{
		/* EOF or error */
		if (isatty(STDIN_FILENO))
			printf("\n");
		if (*command != NULL)
			free(*command);
		exit(EXIT_SUCCESS);
	}
	if ((*command)[read_bytes - 1] == '\n')
		(*command)[read_bytes - 1] = '\0';
}

/**
 * main - shell entrypoint (interactive or non-interactive)
 * @argc: arg count
 * @argv: arg vector
 * @envp: environment variables
 *
 * Return: 0 on success, or exit with failure
 */
int main(int argc, char *argv[], char **envp)
{
	size_t size;

	(void)argv;
	env = envp;
	command = NULL;

	if (argc > 1)
	{
		fprintf(stderr, "./shell: command does not exist\n");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		size = 0;
		if (isatty(STDIN_FILENO))
			printf("shell($) ");
		input(&command, &size);

		if (strcmp(command, "exit") == 0)
		{
			free(command);
			exit(EXIT_SUCCESS);
		}

		if (strcmp(command, "env") == 0)
		{
			print_env();
			free(command);
			continue;
		}

		/* If PATH isn't set and command is not an absolute path, it's not found */
		if (_getenv("PATH", envp) == NULL && command[0] != '/')
		{
			fprintf(stderr, "./hsh: 1: %s: not found\n", command);
			free(command);
			continue;
		}

		parse(command, envp);
		free(command);
		command = NULL;
	}
	return (0);
}
