#include <thread_pool.h>

void moo_test(void* args) {
    size_t num = (size_t) args;
    size_t big_num = 1 << 25;
    printf("im in the function\n");
    for(size_t i = 0; i < num; i++) {
        big_num -= i;
    }
}

int main() {
    tpool_t* tpool = tpool_init(6);

    for(size_t i = 1; i < 69; i++) {
        tpool_work_add(tpool, moo_test, (void*)i);
    }

    tpool_wait(tpool);

    tpool_destroy(tpool);

    return 0;
}
