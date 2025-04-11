#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int arr1[] = {1, 2};
    short int arr2[] = {1, 2};

    if (!memcmp(arr1, arr2, 2 * sizeof(short int)))
        printf("Arrays are equal\n");
    else
        printf("Arrays are not equal\n");

    return 0;
}