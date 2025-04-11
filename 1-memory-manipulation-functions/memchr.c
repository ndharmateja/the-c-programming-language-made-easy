#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int arr1[] = {1, 2};

    // searches for a byte in memory
    if (memchr(arr1, 0x03, 2 * sizeof(int)))
        printf("3 is in the array\n");
    else
        printf("3 is not in the array\n");

    return 0;
}