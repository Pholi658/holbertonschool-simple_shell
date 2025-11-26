#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

/* Core functions */
char *read_line(void);
char **split_line(char *line);
int execute_command(char **args, char *prog_name, unsigned int *line_count);
void free_args(char **args);
char *find_executable(char *cmd);

/* Prompt */
void print_prompt(void);

#endif /* SHELL_H */

