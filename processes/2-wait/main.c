#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();
    int n;
    if (pid == 0)
    {
        for (int i = 0; i < 5; i++)
        {
            printf("child: %d\n", i);
            sleep(1);
        }
    }
    else
    {
        // Stop executing until one of the children is done
        wait(NULL);

        for (int i = 0; i < 5; i++)
        {
            printf("parent: %d\n", i);
            sleep(1);
        }
    }
    return 0;
}