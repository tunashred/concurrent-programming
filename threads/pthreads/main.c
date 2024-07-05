#include <pthread.h>
#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>

pthread_mutex_t lock_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_b = PTHREAD_MUTEX_INITIALIZER;

int x = 0;

void* lock_reader(void* mutex) {
    int en;
    int new_x = - 1,
        old_x = -1;
    pthread_mutex_t* p_mutex = (pthread_mutex_t *) mutex;

    pthread_mutex_lock(p_mutex);

    for(int i = 0; i < 100; i++) {
        new_x = x;
        if(new_x != old_x) {
            printf("lock_reader(): new x val: %d\n", new_x);
        }

        old_x = new_x;

        poll(NULL, 0, 1);
    }

    pthread_mutex_unlock(p_mutex);

    return NULL;
}

void* lock_writer(void* mutex) {
    int en;
    pthread_mutex_t* p_mutex = (pthread_mutex_t *) mutex;

    pthread_mutex_lock(p_mutex);

    for(int i = 0; i < 3; i++) {
        x++;
        poll(NULL, 0, 5);
    }

    pthread_mutex_unlock(p_mutex);

    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    printf("Creating two threads using same lock:\n");
    pthread_create(&thread1, NULL, lock_reader, &lock_a);
    pthread_create(&thread2, NULL, lock_writer, &lock_a);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("\nCreating two threads with different locks:\n");
    pthread_create(&thread1, NULL, lock_reader, &lock_a);
    pthread_create(&thread2, NULL, lock_writer, &lock_b);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&lock_a);
    pthread_mutex_destroy(&lock_b);

    return 0;
}