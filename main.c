#include "shell.h"

char **env = NULL;

/**
 * print_env - prints the environment
 * Return: 0 on success
 */
int print_env(void)
{
    int i = 0;

    if (!env)
        return -1;

    while (env[i])
        printf("%s\n", env[i++]);

    return 0;
}

/**
 * parse - tokenizes command and executes
 * @command: input line
 * @envp: environment
 */
void parse(char *command, char **envp)
{
    char *arguments[11];
    char *token;
    int arg_count = 0;
    int status;

    if (!command || !*command)
        return;

    token = strtok(command, " ");
    while (token && arg_count < 10)
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

    if (arg_count == 0)
        return;

    /* Check for built-ins */
    if (strcmp(arguments[0], "exit") == 0)
    {
        exit(EXIT_SUCCESS);
    }
    else if (strcmp(arguments[0], "env") == 0)
    {
        print_env();
        return;
    }

    /* Execute command */
    status = execute(arguments, envp);

    /* Command not found handling */
    if (status == 127)
        exit(127);
}

/**
 * input - reads a line from stdin
 * @command: pointer to buffer
 * @size: pointer to buffer size
 */
void input(char **command, size_t *size)
{
    ssize_t read_bytes;

    read_bytes = getline(command, size, stdin);
    if (read_bytes == -1)
    {
        if (isatty(STDIN_FILENO))
            printf("\n");
        if (*command)
            free(*command);
        exit(EXIT_SUCCESS);
    }

    if ((*command)[read_bytes - 1] == '\n')
        (*command)[read_bytes - 1] = '\0';
}

/**
 * main - simple shell
 * @argc: arg count
 * @argv: arg vector
 * @envp: environment
 * Return: 0
 */
int main(int argc, char *argv[], char *envp[])
{
    char *command = NULL;
    size_t size = 0;

    (void)argv;
    env = envp;

    if (argc > 1)
    {
        fprintf(stderr, "./shell: command does not exist\n");
        return EXIT_FAILURE;
    }

    while (1)
    {
        if (isatty(STDIN_FILENO))
            printf("shell($) ");
        fflush(stdout);

        input(&command, &size);
        parse(command, envp);

        if (command)
            free(command);
        command = NULL;
        size = 0;
    }

    return 0;
}
