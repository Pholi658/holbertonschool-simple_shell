#include "shell.h"

/* Duplicate string */
char *_strdup(char *str)
{
    int len = 0, i;
    char *dup;

    if (!str)
        return (NULL);

    while (str[len])
        len++;

    dup = malloc(len + 1);
    if (!dup)
        return (NULL);

    for (i = 0; i < len; i++)
        dup[i] = str[i];
    dup[len] = '\0';

    return (dup);
}

/* String length */
size_t _strlen(char *s)
{
    size_t len = 0;

    while (s[len])
        len++;
    return (len);
}

/* String copy */
char *_strcpy(char *dest, char *src)
{
    int i = 0;

    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return (dest);
}

/* String concatenation */
char *_strcat(char *dest, char *src)
{
    int i = 0, j = 0;

    while (dest[i])
        i++;

    while (src[j])
        dest[i++] = src[j++];

    dest[i] = '\0';
    return (dest);
}

/* String compare */
int _strcmp(char *s1, char *s2)
{
    int i = 0;

    while (s1[i] && s2[i])
    {
        if (s1[i] != s2[i])
            return (s1[i] - s2[i]);
        i++;
    }
    return (s1[i] - s2[i]);
}

