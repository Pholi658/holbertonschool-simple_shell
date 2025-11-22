#include "shell.h"
#include <string.h>
#include <sys/wait.h>

/**
 * main - Entry point for simple_shell
 *
 * Return: Always 0
 */
int main(void)
{
    char *line;
    char *args[64];
    char *token;
    int i;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            printf("#cisfun$ ");

        line = read_line();
        if (line == NULL) /* Ctrl+D */
        {
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        /* Split line into args */
        i = 0;
        token = strtok(line, " \t\n");
        while (token != NULL && i < 63)
        {
            args[i++] = token;
            token = strtok(NULL, " \t\n");
        }
        args[i] = NULL;

        if (args[0] == NULL)
        {
            free(line);
            continue;
        }

        if (strcmp(args[0], "exit") == 0)
        {
            free(line);
            break;
        }

        execute_command(args);
        free(line);
    }

    return (0);
}
