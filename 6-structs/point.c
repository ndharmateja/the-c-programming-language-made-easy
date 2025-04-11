#include <stdio.h>
#include <stdlib.h>

typedef struct Point
{
    int x;
    int y;
} Point;

void printPoint(Point p)
{
    printf("p.x: %d\n", p.x);
    printf("p.y: %d\n", p.y);
}

int main()
{
    Point p1 = {1, 2};
    printPoint(p1);

    Point p2 = {
        .x = 3,
        .y = 4,
    };
    printPoint(p2);

    Point p3;
    p3.x = 5;
    p3.y = 6;
    printPoint(p3);

    return 0;
}
