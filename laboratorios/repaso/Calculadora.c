#include <stdio.h>

int main() {
    float num1, num2, resultado;
    char operacion;

    printf("Ingrese el primer número: ");
    scanf("%f", &num1);

    printf("Ingrese el segundo número: ");
    scanf("%f", &num2);

    printf("Seleccione la operación (+, -, *, /): ");
    scanf(" %c", &operacion);

    switch (operacion) {
        case '+':
            resultado = num1 + num2;
        printf("Resultado: %.2f\n", resultado);
        break;
        case '-':
            resultado = num1 - num2;
        printf("Resultado: %.2f\n", resultado);
        break;
        case '*':
            resultado = num1 * num2;
        printf("Resultado: %.2f\n", resultado);
        break;
        case '/':
            if (num2 != 0) {
                resultado = num1 / num2;
                printf("Resultado: %.2f\n", resultado);
            } else {
                printf("Error: División por cero no permitida.\n");
            }
        break;
        default:
            printf("Operación no válida. Por favor use +, -, * o /.\n");
    }

    return 0;
}
