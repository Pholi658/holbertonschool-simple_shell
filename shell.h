#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

char *read_line(void);
void execute_command(char **args);
char *trim_whitespace(char *str);

#endif /* SHELL_H */
