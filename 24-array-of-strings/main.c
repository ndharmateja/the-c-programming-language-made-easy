#include <stdio.h>
#include <string.h>

int main()
{
    char arr[][20] = {
        "hello",
        "world",
        "hello world",
    };
    printf("Array 1:\n");
    for (int i = 0; i < 3; i++)
    {
        printf("%s\n", arr[i]);
    }

    char arr2[3][20];
    strcpy(arr2[0], "hello");
    strcpy(arr2[1], "world");
    strcpy(arr2[2], "hello world");
    printf("Array 2:\n");
    for (int i = 0; i < 3; i++)
    {
        printf("%s\n", arr2[i]);
    }

    char arr3[3 * 20];
    strcpy(arr3, "hello");
    strcpy(arr3 + 20, "world");
    strcpy(arr3 + 40, "hello world");
    printf("Array 3:\n");
    for (int i = 0; i < 3; i++)
    {
        printf("%s\n", arr3 + i * 20);
    }

    return 0;
}