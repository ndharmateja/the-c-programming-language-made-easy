#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TEN_MILLION 10000000

int mails = 0;

// 1. Two threads running in parallel
// mails should be equal to 20000000 at the end
// but it would most probably be much less due to race conditions
// Some eg results: 10004313, 13590138, 10002859, 10388443, 12107813 etc
// 2. As mails++ is broke down into 3-4 ops at assembly level (read, increment, write back etc)
// the operations could be interleaved
// 3. These race conditions might not occur for smaller number of iterations as the
// first thread might get finished even before the second thread is created and run
// 4. Race conditions can occur only in multicore processors
void *f()
{
    for (int i = 0; i < TEN_MILLION; i++)
        mails++;
    return NULL;
}

int main()
{
    pthread_t t1, t2;
    if (pthread_create(&t1, NULL, &f, NULL) != 0 ||
        pthread_create(&t2, NULL, &f, NULL) != 0)
        return 1;

    if (pthread_join(t1, NULL) != 0 || pthread_join(t2, NULL) != 0)
        return 2;

    printf("Mails: %d\n", mails);
    return 0;
}