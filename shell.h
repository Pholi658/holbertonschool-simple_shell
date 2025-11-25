#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern char **environ;

char *read_line(void);
char **split_line(char *line);
void execute_command(char **args);
char *find_command(char *cmd);

#endif
