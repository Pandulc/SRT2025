#include <stdio.h>

void ordenar(int arr[], int n, int ascendente) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if ((ascendente && arr[j] > arr[j + 1]) || (!ascendente && arr[j] < arr[j + 1])) {
                // Intercambio de elementos
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n;

    printf("Ingrese la cantidad de números: ");
    scanf("%d", &n);

    int numeros[n];
    for (int i = 0; i < n; i++) {
        printf("Ingrese el número %d: ", i + 1);
        scanf("%d", &numeros[i]);
    }

    char orden;
    printf("¿Desea ordenarlos en forma ascendente (A) o descendente (D)? ");
    scanf(" %c", &orden);

    if (orden == 'A' || orden == 'a') {
        ordenar(numeros, n, 1);
        printf("Números ordenados en forma ascendente: ");
    } else if (orden == 'D' || orden == 'd') {
        ordenar(numeros, n, 0);
        printf("Números ordenados en forma descendente: ");
    } else {
        printf("Opción no válida. Finalizando programa.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("%d ", numeros[i]);
    }
    printf("\n");

    return 0;
}
