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

char* buffer[] = {"john", "carmack", "jonathan", "blow", "jim", "keller"};
pthread_rwlock_t rw_lock;

void* read_buffer() {
    pthread_rwlock_rdlock(&rw_lock);
    printf("%s\n", buffer[0]);
    pthread_rwlock_unlock(&rw_lock);

    // sleep(1);

    pthread_rwlock_rdlock(&rw_lock);
    printf("%s\n", buffer[0]);
    pthread_rwlock_unlock(&rw_lock);
    return NULL;
}

void* write_buffer() {
    pthread_rwlock_wrlock(&rw_lock);
    buffer[0] = "69";
    pthread_rwlock_unlock(&rw_lock);
    return NULL;
}

int main() {
    pthread_t reader1, reader2, writer;
    pthread_rwlockattr_t rwlock_attr;
    pthread_rwlockattr_init(&rwlock_attr);
    pthread_rwlockattr_setkind_np(&rwlock_attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);

    pthread_rwlock_init(&rw_lock, &rwlock_attr);
    pthread_rwlockattr_destroy(&rwlock_attr);
    
    pthread_create(&reader1, NULL, read_buffer, NULL);
    pthread_create(&reader2, NULL, read_buffer, NULL);
    pthread_create(&writer, NULL, write_buffer, NULL);

    pthread_join(reader1, NULL);
    pthread_join(reader2, NULL);
    pthread_join(writer, NULL);

    pthread_rwlock_destroy(&rw_lock);

    return 0;
}
