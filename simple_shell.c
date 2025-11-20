#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ; /* Required for execve */

int main(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    pid_t pid;

    while (1)
    {
        printf("#cisfun$ ");
        if (getline(&line, &bufsize, stdin) == -1)
        {
            printf("\n");
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, "exit") == 0)
            break;

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }

        if (pid == 0) /* child process */
        {
            char *args[2];
            args[0] = line;
            args[1] = NULL;

            execve(line, args, environ);
            perror(line); /* only prints if execve fails */
            exit(EXIT_FAILURE);
        }
        else /* parent process */
        {
            wait(NULL);
        }
    }

    free(line);
    return 0;
}
