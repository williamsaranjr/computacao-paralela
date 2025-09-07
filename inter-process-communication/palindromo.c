#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char* inverter(char palavra[]) {
    int n = strlen(palavra);
    char *palavra_invertida = malloc(n + 1);

    for (int i = 0; i < n; i++) {
        palavra_invertida[i] = palavra[n - 1 - i];
    }
    palavra_invertida[n] = '\0';

    return palavra_invertida;
}

bool is_a_palindrome(char palavra[]) {
    int i = 0;
    int j = strlen(palavra) - 1;

    while (i < j) {
        if (palavra[i] != palavra[j]) {
            return false;
        }

        i++;
        j--;
    }

    return true;
}
