#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("[%d] Start\n", getpid());
    pid_t fork_result = fork();
    if (fork_result == -1)
    {
        printf("[%d] fork_result: %d\n", getpid(), fork_result);
        return 1; 
    }
    if (fork_result == 0)
    {
        printf("[%d] fork_result: %d\n", getpid(), fork_result);
    }
    else
    {
        printf("[%d] fork_result: %d\n", getpid(), fork_result);
    }
    return 0;
}