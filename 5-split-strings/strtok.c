#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char str[] = "Let's break up this string";

    char *token = strtok(str, " ");
    while (token != NULL)
    {
        printf("token: %s\n", token);
        token = strtok(NULL, " ");
    }

    return 0;
}