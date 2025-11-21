#include "shell.h"

/**
 * print_prompt - display shell prompt
 */
void print_prompt(void)
{
    if (isatty(STDIN_FILENO))
        write(STDOUT_FILENO, "$ ", 2);
}

