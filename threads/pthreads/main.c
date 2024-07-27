#include <pthread.h>
#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>

#define MAX_SLEEP 10
#define MAX_THREADS 2

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* callback1(void* arg) {
    int random_time;
    while(69) {
        srand(time(NULL));
        random_time = rand() % MAX_SLEEP;
        printf("[%s]: Sleeping for %d seconds\n", __FUNCTION__, random_time);
        sleep(random_time);
        printf("[%s]: Trying to acquire mutex1 (holding none)\n", __FUNCTION__);
        pthread_mutex_lock(&mutex1);
        printf("[%s]: Acquired mutex1\n", __FUNCTION__);

        random_time = rand() % MAX_SLEEP;
        printf("[%s]: Sleeping for %d seconds\n", __FUNCTION__, random_time);
        sleep(random_time);

        printf("[%s]: Trying to acquire mutex2 (holding mutex1)\n", __FUNCTION__);
        pthread_mutex_lock(&mutex2);
        printf("[%s]: Acquired mutex2\n\n", __FUNCTION__);

        pthread_mutex_unlock(&mutex2);
        pthread_mutex_unlock(&mutex1);
    }
    return NULL;
}

void* callback2(void* arg) {
    int random_time;
    while(69) {
        srand(time(NULL));
        random_time = rand() % MAX_SLEEP;
        printf("[%s]: Sleeping for %d seconds\n", __FUNCTION__, random_time);
        sleep(random_time);
        printf("[%s]: Trying to acquire mutex2 (holding none)\n", __FUNCTION__);
        pthread_mutex_lock(&mutex2);
        printf("[%s]: Acquired mutex2\n", __FUNCTION__);

        random_time = rand() % MAX_SLEEP;
        printf("[%s]: Sleeping for %d seconds\n", __FUNCTION__, random_time);
        sleep(random_time);

        printf("[%s]: Trying to acquire mutex1 (holding mutex2)\n", __FUNCTION__);
        pthread_mutex_lock(&mutex1);
        printf("[%s]: Acquired mutex1\n\n", __FUNCTION__);

        pthread_mutex_unlock(&mutex1);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
}

int main() {
    pthread_t thread[MAX_THREADS];

    pthread_create(&thread[0], NULL, callback1, NULL);
    pthread_create(&thread[1], NULL, callback2, NULL);
    
    for(int i = 0; i < MAX_THREADS; i++) {
        pthread_join(thread[i], NULL);
    }

    return 0;
}