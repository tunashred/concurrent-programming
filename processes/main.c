#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void handler(int sig) {
    printf("Interrupt handler has just been called\n");

    signal(SIGINT, handler);
}

int main() {
    signal(SIGINT, handler);

    int i = 0;
    while(i < 10) {
        printf("ceau ceau\n");
        sleep(1);
        i++;
    }

    return 0;
}