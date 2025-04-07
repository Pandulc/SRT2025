#include <stdio.h>
#define SHRT_MAX 32767
#define SHRT_MIN (-32768)

int main() {
    float floatVar;
    short shortVar;

    // Solicitar al usuario que ingrese un número de punto flotante
    printf("Ingrese un número de punto flotante: ");
    scanf("%f", &floatVar);

    // Verificar si el valor está dentro del rango de short
    if (floatVar > SHRT_MAX) {
        printf("El valor ingresado es mayor que el máximo de short. Asignando SHRT_MAX.\n");
        shortVar = SHRT_MAX;
    } else if (floatVar < SHRT_MIN) {
        printf("El valor ingresado es menor que el mínimo de short. Asignando SHRT_MIN.\n");
        shortVar = SHRT_MIN;
    } else {
        // Convertir el float a short si está dentro del rango
        shortVar = (short)floatVar;
    }

    // Imprimir los valores
    printf("Float ingresado: %f\n", floatVar);
    printf("Short resultante: %d\n\n", shortVar);

    printf("Ingrese un valor short: ");
    scanf("%d", &shortVar);

    floatVar = (float)shortVar;
    printf("Short ingresado: %d\n", shortVar);
    printf("float resultante: %.2f", floatVar);

    return 0;
}