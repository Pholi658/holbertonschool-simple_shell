#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *line;
    char **args;
    unsigned int line_count = 0;

    while (1)
    {
        print_prompt();           
        line = read_line();      
        if (!line)
            break;               

        args = split_line(line);  
        free(line);               

        if (!args)
            continue;

        execute_command(args, &line_count); 
        free_args(args);                   
    }

    return 0;
}

