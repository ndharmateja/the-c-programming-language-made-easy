#include <stdio.h>

// Struct: Group (32 bytes total, 64-bit system)
// Byte-by-byte layout:
// +------+------+------+------+------+------+------+------+
// |  0   |  1   |  2   |  3   |  4   |  5   |  6   |  7   |
// |  x0  |  x1  |  x2  |  x3  |  c   | str0 | str1 | str2 |
// +------+------+------+------+------+------+------+------+
// |  8   |  9   | 10   | 11   | 12   | 13   | 14   | 15   |
// | str3 | str4 | str5 | str6 | str7 | str8 | str9 | pad  |
// +------+------+------+------+------+------+------+------+
// | 16   | 17   | 18   | 19   | 20   | 21   | 22   | 23   |
// |   p pointer (8-byte aligned memory address)           |
// +------+------+------+------+------+------+------+------+
// | 24   | 25   | 26   | 27   | 28   | 29   | 30   | 31   |
// | y0   | y1   |     padding (6 bytes for alignment)     |
// +------+------+------+------+------+------+------+------+

// Legend:
// - `x0`..`x3` : 4 bytes of `int x`
// - `c`       : 1 byte `char c`
// - `str0`..`str9` : 10 bytes of `char str[10]`
// - `pad`     : 1 byte padding after str to align pointer
// - `p`       : 8 bytes of pointer `int *p`, aligned at byte 16
// - `y0`..`y1`: 2 bytes of `short int y`
// - final padding: 6 bytes to align total struct size to 8 bytes
typedef struct
{
    int x;        // 4
    char c;       // 1
    char str[10]; // 10
    int *p;       // 8
    short int y;  // 2
} Group;

int main()
{
    int x;
    Group g = {
        .x = 12,
        .c = 'k',
        .str = "testing",
        .p = &x,
        .y = 256,
    };
    printf("sizeof(Group): %lu\n", sizeof(Group));

    for (int i = 0; i < sizeof(Group); i++)
    {
        printf("%02x ", *(((unsigned char *)&g) + i));
        if (i % 8 == 7)
            printf("\n");
    }

    return 0;
}