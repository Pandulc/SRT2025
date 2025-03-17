#include <stdio.h>
#include <pigpio.h>

#define PIN_BOTON 21
#define PIN_LED 19

// Función de interrupción para el botón
void botonCallback(int gpio, int level, uint32_t tick) {
    if (level == 1) {
        printf("BOTÓN_PRESIONADO\n");
        gpioWrite(PIN_LED, 1);  // Enciende el LED
    } else if (level == 0) {
        printf("BOTÓN_LIBERADO\n");
        gpioWrite(PIN_LED, 0);  // Apaga el LED
    }
}

int main() {
    // Inicializa la biblioteca pigpio
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Error al inicializar pigpio\n");
        return 1;
    }

    // Configurar el LED como salida y el botón como entrada
    gpioSetMode(PIN_LED, PI_OUTPUT);
    gpioSetMode(PIN_BOTON, PI_INPUT);

    // Habilitar la resistencia pull-down en el botón (para asegurar nivel bajo por defecto)
    gpioSetPullUpDown(PIN_BOTON, PI_PUD_DOWN);

    // Configurar la interrupción en el botón (ambos flancos)
    gpioSetAlertFunc(PIN_BOTON, botonCallback);

    printf("Presiona Ctrl+C para salir.\n");

    // Mantener el programa en ejecución
    while (1) {
        time_sleep(1.0);  // Espera para reducir el uso de CPU
    }

    gpioTerminate();  // Finaliza la biblioteca pigpio
    return 0;
}
