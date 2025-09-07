#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "palindromo.h"

#define NUMERO_DE_PIPES 3
#define TAMANHO_BUFFER 256

int pipes[NUMERO_DE_PIPES][2];
char buffer[TAMANHO_BUFFER];

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("[ERRO] Argumentos foram passados incorretamente. Tente novamente com:\n\t <PALAVRA>\n");

        return 1;
    }

    for (int i = 0; i < NUMERO_DE_PIPES; i++) {
        if (pipe(pipes[i]) == -1) {
            printf("[ERRO] Houve um erro ao inicializar a pipe %d.\n", i + 1);

            return 1;
        }
    }

    int pid = fork();

    if (pid == 0) { // Processo filho
        close(pipes[0][1]); // Fechar o primeiro pipe de escrita
        close(pipes[1][0]); // Fechar o segundo pipe de leitura
        close(pipes[2][0]); // Fechar o terceiro pipe de leitura

        printf("[FILHO %1s %5d] Esperando palavra no 1° pipe\n", "@", getpid());
        // Ler a palavra no primeiro pipe
        read(
            pipes[0][0], // Primeiro conjunto de pipes - pipe de leitura
            buffer,
            sizeof(buffer)
        );

        printf("[FILHO %1s %5d] Palavra recebida no 1° pipe: %s\n", "@", getpid(), buffer);
        char *palavra_invertida = inverter(buffer);
        printf("[FILHO %1s %5d] Palavra invertida: %s\n", "@", getpid(), palavra_invertida);

        // Escrever a palavra invertida no segundo pipe
        printf("[FILHO %1s %5d] Escrevendo palavra invertida no 2° pipe\n", "@", getpid());
        write(
            pipes[1][1], // Primeiro conjunto de pipes - pipe de escrita
            palavra_invertida, // Argumento de linha de comando
            strlen(palavra_invertida)
        );
        printf("[FILHO %1s %5d] Palavra escrita com sucesso no 2° pipe\n", "@", getpid());

        char *retorno = is_a_palindrome(buffer) == true ? "É um políndromo" : "NÃO É um políndromo";
        printf("[FILHO %1s %5d] Palavra '%s' %s\n", "@", getpid(), buffer, retorno);

        printf("[FILHO %1s %5d] Escrevendo resultado no 3° pipe\n", "@", getpid());
        write(
            pipes[2][1], // Primeiro conjunto de pipes - pipe de escrita
            retorno, // Argumento de linha de comando
            strlen(retorno)
        );
        printf("[FILHO %1s %5d] O resultado '%s' foi escrito com sucesso no 3° pipe\n", "@", getpid(), retorno);
    } else { // Processo pai
        close(pipes[0][0]); // Fechar o primeiro pipe de leitura
        close(pipes[1][1]); // Fechar o segundo pipe de escrita
        close(pipes[2][1]); // Fechar o segundo pipe de escrita

        printf("[PAI %3s %5d] Palavra passada como argumento: %s\n", "@", getpid(), argv[1]);
        printf("[PAI %3s %5d] Escrevendo a palavra '%s' no 1° pipe\n", "@", getpid(), argv[1]);
        // Escrever a palavra no primeiro pipe
        write(
            pipes[0][1], // Primeiro conjunto de pipes - pipe de escrita
            argv[1], // Argumento de linha de comando
            strlen(argv[1])
        );
        printf("[PAI %3s %5d] Palavra '%s' foi escrita com sucesso no 1° pipe\n", "@", getpid(), argv[1]);


        // Ler a palavra no segundo pipe
        printf("[PAI %3s %5d] Esperando a palavra invertida no 2° pipe\n", "@", getpid());
        read(
            pipes[1][0], // Primeiro conjunto de pipes - pipe de leitura
            buffer,
            sizeof(buffer)
        );
        printf("[PAI %3s %5d] Palavra invertida recebida no 2° pipe: %s\n", "@", getpid(), buffer);

        // Ler o resulatdo no terceiro pipe
        printf("[PAI %3s %5d] Esperando o resultado no 3° pipe\n", "@", getpid());
        read(
            pipes[2][0], // Primeiro conjunto de pipes - pipe de leitura
            buffer,
            sizeof(buffer)
        );
        printf("[PAI %3s %5d] Resultado recebido no 3° pipe: %s\n", "@", getpid(), buffer);
    }

    return 0;
}
