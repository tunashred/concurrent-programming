#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// for pipes:
// write - 0
// read  - 1
int main() {
    pid_t pid;
    int descriptor[2];
    int data;
    char message[69] = "Azi mananc peste proaspat";
    char buffer[420];

    pipe(descriptor);

    if( (pid = fork()) < 0) {
        fprintf(stderr, "Unable to create new process\n");
        exit(1);
    }

    if(pid == 0) {
        close(descriptor[1]);
        data = read(descriptor[0], buffer, sizeof(buffer));
        printf("Printing data value: %d\n", data);
        printf("Message received from parent: %s\n", buffer);
    }
    else {
        close(descriptor[0]);
        sleep(2);
        write(descriptor[1], message, strlen(message) + 1);
        wait(0);
    }

    return 0;
}