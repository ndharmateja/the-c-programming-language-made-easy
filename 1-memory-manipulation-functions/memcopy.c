#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int arr1[] = {1, 2};
    int arr2[] = {3, 4};

    printf("arr1: ");
    for (int i = 0; i < 2; i++)
    {
        printf("%d ", arr1[i]);
    }
    printf("\n");

    printf("arr2: ");
    for (int i = 0; i < 2; i++)
    {
        printf("%d ", arr2[i]);
    }
    printf("\n");

    memcpy(arr1, arr2, 2 * sizeof(int));

    printf("arr1: ");
    for (int i = 0; i < 2; i++)
    {
        printf("%d ", arr1[i]);
    }
    printf("\n");

    printf("arr2: ");
    for (int i = 0; i < 2; i++)
    {
        printf("%d ", arr2[i]);
    }
    printf("\n");

    return 0;
}