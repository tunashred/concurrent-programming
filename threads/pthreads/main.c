#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <bits/pthreadtypes.h>
#include <poll.h>
#include <time.h>

#define lock_mutex(x) do { pthread_mutex_lock(x); lock_count++; } while (0)
#define unlock_mutex(x) do { pthread_mutex_unlock(x); lock_count--; } while (0)

pthread_mutex_t mutex;
static int x = 0;
__thread int lock_count = 0;

void mutex_init(pthread_mutex_t* mutex, int kind) {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, kind);
    pthread_mutex_init(mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

void* counter() {
    int i = 0;
    for(i = 0; i < 5; i++) {
        lock_mutex(&mutex);
    }
    printf("Number of max recursive locks: %d\n", lock_count);
    x++;
    sleep(3);
    for(i = 0; i < 5; i++) {
        printf("Number of recursive locks: %d\n", lock_count);
        unlock_mutex(&mutex);
    }

    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    mutex_init(&mutex, PTHREAD_MUTEX_RECURSIVE);

    pthread_create(&thread1, NULL, counter, NULL);
    pthread_create(&thread2, NULL, counter, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}