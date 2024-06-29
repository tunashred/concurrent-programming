#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t pid;
    char *args_ls[] = {"ls", "-la", "/", 0};
    char *env_ls[] = {0};

    if((pid = fork()) < 0) {
        fprintf(stderr, "Unable to fork a new process\n");
    }
    
    if(pid == 0) {
        execve("/bin/ls", args_ls, env_ls);
    }
    else {
        printf("420\n");
    }
    

    return 0;
}