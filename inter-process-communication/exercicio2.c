#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "palindromo.h"

#define BUF_SIZE 1024
#define SHM_KEY 0x1234

typedef struct {
    bool ready;
    bool done;
    bool is_pal;
    char text[BUF_SIZE];
} shm_data;

int main(int argc, char *argv[]) {
    int shmid = shmget(IPC_PRIVATE, sizeof(shm_data), IPC_CREAT | 0600); // Cria o espaço endereçável
    shm_data *shm = (shm_data *) shmat(shmid, NULL, 0); // Anexa à tabela de páginas

    shm->ready = false;
    shm->done  = false;
    shm->is_pal = false;

    if (argc != 2) {
        printf("[ERRO] Argumentos foram passados incorretamente. Tente novamente com:\n\t <PALAVRA>\n");

        return 1;
    }

    int pid = fork();

    if (pid == 0) {
        while (!shm->ready) {
            printf("[FILHO] A palavra ainda não foi disponibilizada.\n");

            sleep(1);
        }

        printf("[FILHO] Palavra '%s' lida com sucesso\n", shm->text);
        printf("[FILHO] Avaliando palavra '%s'\n", shm->text);

        bool resultado = is_a_palindrome(shm->text);
        shm->is_pal = resultado;
        printf("[FILHO] Palavra '%s' %s - Alterando flags em 3 segundos...\n", shm->text, resultado ? "É um palíndromo" : "NÃO É um palíndromo");
        sleep(3);

        shm->done = true;

        printf("[FILHO] Flag alterada com sucesso\n");
    } else {
        printf("[PAI] Escrevendo a palavra '%s'\n", argv[1]);

        strncpy(shm->text, argv[1], BUF_SIZE - 1);
        printf("[PAI] Palavra '%s' escrita com sucesso - Alterando flag em 3 segundos...\n", shm->text);
        sleep(3);

        shm->ready = true;
        printf("[PAI] Flag alterada com sucesso\n");

        while (!shm->done) {
            printf("[PAI] O resultado ainda não foi disponibilizado.\n");

            sleep(1);
        }

        printf("[PAI] Flag lida com sucesso\n");

        printf("[PAI] Palavra '%s' %s\n", shm->text, shm->is_pal ? "É um palíndromo" : "NÃO É um palíndromo");
    }

    return 0;
}
