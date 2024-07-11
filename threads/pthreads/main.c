#include <pthread.h>
#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_r = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_w = PTHREAD_COND_INITIALIZER;

char buffer[1024];
int buf_count = 0, buf_r_loc = 0, buf_w_loc = 0;

#define INC(x) ++x == 1024 ? x = 0 : x

void* read_buffer() {
    char out;
    pthread_mutex_lock(&mutex);

    while(buf_count == 0) {
        pthread_cond_broadcast(&cond_w);
        pthread_cond_wait(&cond_r, &mutex);
    }
    buf_count--;
    out = buffer[buf_r_loc];
    INC(buf_r_loc);

    pthread_cond_broadcast(&cond_w);
    pthread_mutex_unlock(&mutex);

    return (void*) out;
}

void* write_buffer(void* arg) {
    char ch = (char) arg;
    pthread_mutex_lock(&mutex);

    while(buf_count == 1024) {
        pthread_cond_broadcast(&cond_r);
        pthread_cond_wait(&cond_w, &mutex);
    }
    buf_count++;
    buffer[buf_w_loc] = ch;
    INC(buf_w_loc);
    
    pthread_cond_broadcast(&cond_r);
    pthread_mutex_unlock(&mutex);
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, read_buffer, NULL);
    pthread_create(&thread2, NULL, write_buffer, (void*) 'a');
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);


    return 0;
}