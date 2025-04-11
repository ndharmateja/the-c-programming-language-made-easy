#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getString(char *str)
{
    strcpy(str, "hello");
}

int main()
{
    char str[20];
    getString(str);
    printf("%s\n", str);
    str[0] = 'H';
    printf("%s\n", str);
    return 0;
}