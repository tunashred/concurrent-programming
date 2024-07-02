#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <bits/types/sigset_t.h>

#define LAMAIE "lamaie"

#define PESTE()          { sleep(1); printf("Pestele se frige in tigaie\n"); }
#define PROASPAT()       { PESTE(); PESTE(); }
#define PESTE_PROASPAT() { PROASPAT(); PROASPAT(); }

void pe_alta_data(int sig) {
    printf("N-avem lamaie.\n");
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
            char* rafie[4] = {"pastrav", "sare", "piper", "lamaie"};
            signal(SIGUSR1, azi_mancam);
            signal(SIGABRT, pe_alta_data);
            close(descriptor[0]);

            bool lamaie = false;
            printf("Ia sa vedem ce avem in rafie:\n");

            int data_in = 0;
            char buffer[420];
            for(int i = 0; i < 4; i++) {
                printf("%s\n", rafie[i]);

                strcpy(buffer + data_in, rafie[i]);
                data_in += strnlen(rafie[i], 20) + 1;
                buffer[data_in - 1] = ';';

                if(!strncmp(rafie[i], LAMAIE, 6)) {
                    lamaie = true;
                }
            }
            write(descriptor[1], buffer, data_in);
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
            printf("\n");
            
            char buffer[420];
            int bytes_read = read(descriptor[0], buffer, sizeof(buffer));
            
            char* token = strtok(buffer, ";");
            while(token) {
                printf("Azvarlim in tigaie %s\n", token);
                sleep(1);
                token = strtok(NULL, ";");
            }

            close(descriptor[0]);
        }
    }
    else { // parent process
        int status;
        waitpid(pid_main, &status, 0);
        if(WIFSIGNALED(status)) {
            printf("Lasam pestele pe alta data\n");
            exit(1);
        }

        close(descriptor[0]);
        close(descriptor[1]);
        printf("\n");

        PESTE_PROASPAT();
        printf("Intoarcem pestele pe partea cealalta\n");
        PESTE_PROASPAT();

        char* message[] = {"/usr/bin/cowsay", "Spor la cafelutsa cu peste", NULL};
        execve("/usr/bin/cowsay", message, 0);
    }

    return 0;
}
// for debug run in gdb
// -exec set follow-fork-mode child