#include <unistd.h>
#include "shell.h"

/**
 * free_args - frees a token array
 */
void free_args(char **args)
{
    int i = 0;

    if (!args)
        return;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}

/* Simple string helper functions */
size_t _strlen(char *s)
{
    size_t i = 0;
    while (s[i])
        i++;
    return (i);
}

char *_strdup(char *s)
{
    int len = _strlen(s);
    char *copy = malloc(len + 1);
    int i;

    if (!copy)
        return (NULL);

    for (i = 0; i <= len; i++)
        copy[i] = s[i];
    return (copy);
}

char *_strcpy(char *dest, char *src)
{
    int i = 0;
    while ((dest[i] = src[i]) != '\0')
        i++;
    return (dest);
}

char *_strcat(char *dest, char *src)
{
    int i = 0, j = 0;
    while (dest[i])
        i++;
    while ((dest[i++] = src[j++]) != '\0')
        ;
    return (dest);
}

int _strcmp(char *s1, char *s2)
{
    int i = 0;
    while (s1[i] && s1[i] == s2[i])
        i++;
    return (s1[i] - s2[i]);
}

