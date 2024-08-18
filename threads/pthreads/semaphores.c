#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <bits/pthreadtypes.h>
#include <poll.h>
#include <time.h>
#include <semaphore.h>
#include <errno.h>

#define MAX_CONCURRENT_THREADS 2
sem_t semaphore;

char* buffer[] = {"john", "carmack", "jonathan", "blow", "jim", "keller"};

void* read_buffer() {
    sem_wait(&semaphore);
    printf("%s %s\n", buffer[0], buffer[1]);
    sleep(2);
    sem_post(&semaphore);
    return NULL;
}

void* write_buffer() {
    sem_wait(&semaphore);
    buffer[0] = "69\0";
    sem_post(&semaphore);
    return NULL;
}

int main() {
    sem_init(&semaphore, 0, MAX_CONCURRENT_THREADS);
    pthread_t reader1, reader2, writer;
    
    //TODO: lookup pthread_attr_t
    pthread_create(&reader1, NULL, read_buffer, NULL);
    pthread_create(&reader2, NULL, read_buffer, NULL);
    pthread_create(&writer, NULL, write_buffer, NULL);

    pthread_join(reader1, NULL);
    pthread_join(reader2, NULL);
    pthread_join(writer, NULL);

    printf("%s\n", buffer[0]);

    sem_destroy(&semaphore);

    return 0;
}
