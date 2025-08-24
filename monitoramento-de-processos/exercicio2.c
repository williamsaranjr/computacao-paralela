#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#define MAXIMO_SUBPROCESSOS 5

int main() {
    for (int i = 0; i < MAXIMO_SUBPROCESSOS; i++) {
        int pid = fork();
        int status;

        if (pid == 0) {
            srand(time(NULL) + getpid());
            int tempo = rand() % 10 + 1;
            sleep(tempo);
            return tempo;
        } else {
            waitpid(pid, &status, 0);
            printf("Processo filho (PID %ld) finalizou com status de saída: %d (0x%04x)\n", (long) pid, WEXITSTATUS(status), status);
        }
    }

    return 0;
}