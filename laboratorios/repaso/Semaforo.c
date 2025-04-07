#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

// Duraciones en segundos
#define VERDE 5
#define AMARILLO 2
#define ROJO 5

bool boton_presionado = false;

void *boton_cruce(void *arg) {
    while (1) {
        char tecla = getchar();
        if (tecla == 's' || tecla == 'S') {
            boton_presionado = true;
        }
    }
}

void semaforo() {
    while (1) {
        printf("\033[1;32mLUZ VERDE\033[0m - Presione 's' para cambiar a ROJO\n");
        for (int i = 0; i < VERDE; i++) {
            sleep(1);
            if (boton_presionado) {
                printf("\033[1;33mBotón presionado, cambiando a AMARILLO...\033[0m\n");
                boton_presionado = false;
                break;
            }
        }

        printf("\033[1;33mLUZ AMARILLA\033[0m\n");
        sleep(AMARILLO);

        printf("\033[1;31mLUZ ROJA\033[0m\n");
        sleep(ROJO);
    }
}

int main() {
    pthread_t hilo_boton;
    pthread_create(&hilo_boton, NULL, boton_cruce, NULL);

    semaforo();

    pthread_join(hilo_boton, NULL);
    return 0;
}
