#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#define SERVO_PIN 18

int main() {
    int angle;
    char input[10];

    // Inicializar pigpio
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Error al inicializar pigpio\n");
        return 1;
    }

    printf("Control de Servo SG90 con PWM\n");
    printf("Ingrese ángulo (0-180) o 'q' para salir:\n");

    while (1) {
        printf("Ángulo: ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;

        // Salir si el usuario ingresa 'q'
        if (input[0] == 'q') break;

        // Convertir entrada a ángulo
        angle = atoi(input);

        // Validar el ángulo ingresado
        if (angle < 0 || angle > 180) {
            printf("Ángulo debe estar entre 0 y 180 grados\n");
            continue;
        }

        // Convertir ángulo a microsegundos (500-2500μs)
        int pulsewidth = 500 + (angle * 2000 / 180);

        // Enviar señal PWM al servo
        gpioServo(SERVO_PIN, pulsewidth);

        printf("Posicionando servo a %d grados (%d μs)\n", angle, pulsewidth);
    }

    // Apagar servo y limpiar pigpio
    gpioServo(SERVO_PIN, 0);
    gpioTerminate();

    return 0;
}