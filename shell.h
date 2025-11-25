#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int execute(char *const command[], char **envp);
void input(char **command, size_t *size);
void printerror(char *const command[]);
char **pathfinder(char *cmd, char **command, char **envp);
int print_env(void);
char *_getenv(const char *name, char **envp);
void parse(char command[], char **envp);

#endif
