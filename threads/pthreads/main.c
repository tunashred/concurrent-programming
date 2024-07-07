#include <pthread.h>
#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>

#define M_LOCK(x) pthread_mutex_lock(x)
#define M_UNLOCK(x) pthread_mutex_unlock(x)

pthread_mutex_t lock_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_b = PTHREAD_MUTEX_INITIALIZER;
int a = 0, b = 0;

void* a_b_inc() {
    M_LOCK(&lock_a);
    M_LOCK(&lock_b);
    b = a++;
    printf("a_b_inc(): a = %d, b = %d\n", a, b);
    M_UNLOCK(&lock_b);
    M_UNLOCK(&lock_a);

    return NULL;
}

void* b_a_dec() {
    M_LOCK(&lock_b);
    M_LOCK(&lock_a);
    a = b--;
    printf("b_a_dec(): a = %d, b = %d\n", a, b);
    M_UNLOCK(&lock_a);
    M_UNLOCK(&lock_b);

    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, a_b_inc, NULL);
    pthread_create(&thread2, NULL, b_a_dec, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    

    return 0;
}