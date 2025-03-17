#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LONGITUD 1000

int contarPalabras(const char *cadena) {
    int contador = 0;
    int enPalabra = 0;

    for (int i = 0; cadena[i] != '\0'; i++) {
        if (!isspace(cadena[i])) {
            if (!enPalabra) {
                enPalabra = 1;
                contador++;
            }
        } else {
            enPalabra = 0;
        }
    }

    return contador;
}

int main() {
    char cadena[MAX_LONGITUD];

    printf("Ingrese una cadena de texto: ");
    fgets(cadena, MAX_LONGITUD, stdin);

    // Elimina el salto de línea al final si existe
    cadena[strcspn(cadena, "\n")] = 0;

    int totalPalabras = contarPalabras(cadena);
    printf("Número de palabras en la cadena: %d\n", totalPalabras);

    return 0;
}
