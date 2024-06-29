#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// for pipes:
// write - 1
// read  - 0
int main() {
    pid_t pid;
    int descriptor[2];
    int data = 69;
    char message[69] = "Mai mananc un peste proaspat";
    char letter;

    pipe(descriptor);

    if( (pid = fork()) < 0) {
        fprintf(stderr, "Unable to create new process\n");
        exit(1);
    }

    if(pid == 0) {
        close(descriptor[1]);
        while(data) {
            data = read(descriptor[0], &letter, sizeof(char));
            sleep(1);
            printf("Letter read: %c\n", letter);
        }
    }
    else {
        close(descriptor[0]);
        sleep(2);
        
        for(int i = 0; i < strnlen(message, 69); i++) {
            write(descriptor[1], &message[i], 1);
        }

        close(descriptor[1]);
        wait(0);
    }

    return 0;
}