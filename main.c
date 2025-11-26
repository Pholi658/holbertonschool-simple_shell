#include "shell.h"

int main(int argc, char **argv)
{
    char *line;
    char **args;
    unsigned int line_count = 0;

    (void)argc;

    while (1)
    {
        print_prompt();
        line = read_line();
        if (!line)
            break;
        line_count++;

        args = split_line(line);
        if (!args)
        {
            free(line);
            continue;
        }

        execute_command(args, argv[0], &line_count);
        free_args(args);
        free(line);
    }

    return (0);
}

