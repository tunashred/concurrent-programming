#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>

typedef void(*thread_func_t)(void* args);

typedef struct {
    thread_func_t func;
    void* args;
    struct tpool_work_t* next;
} tpool_work_t;

typedef struct {
    tpool_work_t* work_first;
    tpool_work_t* work_last;
    pthread_mutex_t work_mutex;
    pthread_cond_t work_cond;
    pthread_cond_t working_cond;
    size_t working_count;
    size_t thread_count;
    bool stop;
} tpool_t;

tpool_t* tpool_init(size_t num);

void tpool_destroy(tpool_t* tpool);

bool tpool_add_work(tpool_t* tpool, thread_func_t func, void* args);

void tpool_wait(tpool_t* tpool);

#endif