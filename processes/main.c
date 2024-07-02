#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <bits/types/sigset_t.h>

#define LAMAIE "lamaie"

void pe_alta_data(int sig) {
    printf("N-avem lamaie.\nLasam pestele pe alta data\n");
    exit(1);
}

void azi_mancam(int sig) {
    printf("Ai tot ce iti trebuie sa faci peste\n");
}

int main() {
    int pid_main;

    if( (pid_main = fork()) < 0) {
        fprintf(stderr, "Unable to fork a new process\n");
        exit(1);
    }

    int descriptor[2];
    if(pipe(descriptor) < 0) {
        fprintf(stderr, "Pipe creation error\n");
        exit(0);
    }

    if(pid_main == 0) { // child process
        int pid_child = fork();

        if(pid_child == 0) { // grandchild process
            char* rafie[4] = {"pastrav", "sare", "piper", "rozmarin"};
            signal(SIGUSR1, azi_mancam);
            signal(SIGABRT, pe_alta_data);
            close(descriptor[0]);

            bool lamaie = false;
            printf("Ia sa vedem ce avem in rafie:\n");
            for(int i = 0; i < 4; i++) {
                printf("%s\n", rafie[i]);
                int data_in = sizeof(char) * (strnlen(rafie[i], 20) + 1);
                write(descriptor[1], rafie[i], data_in);
                if(!strncmp(rafie[i], LAMAIE, 6)) {
                    lamaie = true;
                }
            }
            printf("\n");
            close(descriptor[1]);

            if(!lamaie) {
                kill(getppid(), SIGKILL);
                raise(SIGABRT);
            }
            else {
                raise(SIGUSR1);
            }
        }
        else { //child process
            signal(SIGUSR1, azi_mancam);
            close(descriptor[1]);

            int status;
            waitpid(pid_child, &status, 0);
            
            char buffer[20];
            int bytes_read;
            while( (bytes_read = read(descriptor[0], buffer, sizeof(buffer))) > 0 ) {
                buffer[bytes_read] = '\0';
                printf("Azvarlim in tigaie %s\n", buffer);
            }

            close(descriptor[0]);
        }
    }
    else { // parent process
        int status;
        waitpid(pid_main, &status, 0);
        close(descriptor[0]);
        close(descriptor[1]);

        printf("parent process\n");
        wait(NULL);
    }

    // TODO: la final un execve(cowsay, "spor la cafelutsa cu peste") --- 
    return 0;
}

// for debug run in gdb
// -exec set follow-fork-mode child