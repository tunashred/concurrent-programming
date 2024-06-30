#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void handler(int sig) {
    printf("S-a terminat butelia coaie\n");
    exit(1);
}

int main() {
    int pid;

    if((pid = fork()) == 0) {
        sleep(5);
        kill(getppid(), SIGUSR1);
    }
    else {
        signal(SIGUSR1, handler);
        int i = 0;
        while(i < 10) {
            sleep(1);
            printf("Pestele se frige in tigaie\n");
            i++;
        }
    }
}