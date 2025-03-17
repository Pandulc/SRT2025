#include <stdio.h>
#include <pigpio.h>
#include <unistd.h> // Para usleep

#define LED_PIN 17 // Usa el GPIO correspondiente a tu conexión

void toggle_led(int gpio, int *state) {
    *state = !(*state);
    gpioWrite(gpio, *state);
    printf("%s\n", *state ? "LED_ON" : "LED_OFF");
}

int main() {
    if (gpioInitialise() < 0) {
        printf("Error al inicializar pigpio.\n");
        return 1;
    }

    int led_state = 0; // Estado inicial apagado
    gpioSetMode(LED_PIN, PI_OUTPUT);

    while (1) {
        toggle_led(LED_PIN, &led_state);
        gpioDelay(500000); // 500ms de espera
    }

    gpioTerminate();
    return 0;
}