#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* environment variables */
extern char **environ;

/* Core functions */
char *read_line(void);
int execute_command(char **args, unsigned int *line_count);
char *find_executable(char *cmd);
char **split_line(char *line);
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

