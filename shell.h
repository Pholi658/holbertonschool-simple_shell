#ifndef SHELL_H
#define SHELL_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* environ provided by libc */
extern char **environ;

/* Core functions */
char **split_line(char *line);
char *find_executable(char *cmd);
int execute_command(char **args, unsigned int *line_count);
void free_args(char **args);

/* Utils */
int _strcmp(char *s1, char *s2);
char *_strdup(char *str);
size_t _strlen(char *s);
char *_strcat(char *dest, char *src);
char *_strcpy(char *dest, char *src);

/* Prompt */
void print_prompt(void);

#endif /* SHELL_H */

