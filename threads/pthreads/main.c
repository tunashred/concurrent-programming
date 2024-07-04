#include <pthread.h>
#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* increment(void* x) {
    pthread_mutex_lock(&mutex);
    int* x_int = (int*) x;
    for(int i = 0; i < 1000; i++) {
        (*x_int)++;
    }
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

int main() {
    int x = 0;
    pthread_t thread1, thread2, thread3;

    pthread_create(&thread1, NULL, increment, &x);
    pthread_create(&thread2, NULL, increment, &x);
    pthread_create(&thread3, NULL, increment, &x);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("%d\n", x);

    pthread_mutex_destroy(&mutex);
    
    return 0;
}