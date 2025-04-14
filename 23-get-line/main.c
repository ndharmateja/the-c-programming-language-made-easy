#include <stdio.h>
#include <string.h>

int main()
{
    char text[3];
    printf("Enter text: ");
    fgets(text, 3, stdin);

    for (int i = 0; i < 3; i++)
        printf("'%c'\n", text[i]);

    return 0;
}