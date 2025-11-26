#include "shell.h"

void print_prompt(void)
{
    if (isatty(STDIN_FILENO))
        write(STDOUT_FILENO, "($) ", 4);
}

