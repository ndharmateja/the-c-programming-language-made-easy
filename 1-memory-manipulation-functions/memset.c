#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int arr[] = {1, 2};

    printf("arr: ");
    for (int i = 0; i < 2; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    memset(arr, 0xff, 2 * sizeof(int));

    printf("arr: ");
    for (int i = 0; i < 2; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}