#include <stdio.h>
#include <stdbool.h>

bool is_prime(int n)
{
    if (n == 2)
        return true;
    if (n < 2 || n % 2 == 0)
        return false;

    for (size_t i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return false;

    return true;
}

int main()
{
    for (int i = 2; i < 100; i++)
    {
        if (is_prime(i))
            printf("%d: %s\n", i, is_prime(i) ? "prime" : "false");
    }
    return 0;
}