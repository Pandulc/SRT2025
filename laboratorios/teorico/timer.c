#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <pigpio.h>
#include <stdbool.h>

// Definición de pines GPIO para los LEDs (usando numeración BCM)
#define LED1 17   // GPIO17 (pin físico 11)
#define LED2 27   // GPIO27 (pin físico 13)
#define LED3 22   // GPIO22 (pin físico 15)

// Variables globales para el control de las tareas
volatile sig_atomic_t keep_running = 1;
struct timespec start_time;

// Variables globales para el control de los leds
unsigned long led1_on_time = 0;
unsigned long led2_on_time = 0;
unsigned long led3_on_time = 0;
const unsigned long led_on_duration = 90;  // en ms


// Prototipos de funciones
void handle_signal(int sig);
void print_elapsed_time(const char* task_name);
void task1(unsigned long);
void task2(unsigned long);
void task3(unsigned long);
void init_executive(void);
double get_elapsed_time(void);

// Manejador de señal para Ctrl+C
void handle_signal(int sig) {
    keep_running = 0;

    // Apagar LEDs
    gpioWrite(LED1, 0);
    gpioWrite(LED2, 0);
    gpioWrite(LED3, 0);
}


// Obtiene el tiempo transcurrido desde el inicio en segundos
double get_elapsed_time(void) {
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    double elapsed = (current_time.tv_sec - start_time.tv_sec) +
                   (current_time.tv_nsec - start_time.tv_nsec) / 1e9;

    return elapsed;
}

// Imprime el tiempo transcurrido para una tarea
void print_elapsed_time(const char* task_name) {
    printf("%s - Tiempo transcurrido: %.3f s\n", task_name, get_elapsed_time());
}

// Tareas periódicas
void task1(unsigned long current_time) {
    print_elapsed_time("Tarea 1 (100 ms)");
    gpioWrite(LED1, 1);
    led1_on_time = current_time;
}

void task2(unsigned long current_time) {
    print_elapsed_time("Tarea 2 (300 ms)");
    gpioWrite(LED2, 1);
    led2_on_time = current_time;
}

void task3(unsigned long current_time) {
    print_elapsed_time("Tarea 3 (500 ms)");
    gpioWrite(LED3, 1);
    led3_on_time = current_time;
}


// Inicializa el ejecutivo cíclico
void init_executive(void) {
    // Configurar el manejador de señales
    signal(SIGINT, handle_signal);

    // Obtener el tiempo de inicio
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    printf("Ejecutivo cíclico iniciado. Presione Ctrl+C para terminar.\n");
}

int main(void) {

    if (gpioInitialise() < 0) {
        fprintf(stderr, "Error al inicializar pigpio\n");
        return 1;
    }

    gpioSetMode(LED1, PI_OUTPUT);
    gpioSetMode(LED2, PI_OUTPUT);
    gpioSetMode(LED3, PI_OUTPUT);

    init_executive();

    unsigned long period_100 = 100;
    unsigned long period_300 = 300;
    unsigned long period_500 = 500;

    unsigned long next_100ms = period_100;
    unsigned long next_300ms = period_300;
    unsigned long next_500ms = period_500;

    while (keep_running) {
        unsigned long current_time = (unsigned long)(get_elapsed_time() * 1000);

        if (current_time >= next_100ms) {
            task1(current_time);
            next_100ms += period_100;
        }

        if (current_time >= next_300ms) {
            task2(current_time);
            next_300ms += period_300;
        }

        if (current_time >= next_500ms) {
            task3(current_time);
            next_500ms += period_500;
        }

        // Apagar LEDs si ya pasaron 90ms desde que se encendieron
        if (gpioRead(LED1) && current_time - led1_on_time >= led_on_duration)
            gpioWrite(LED1, 0);

        if (gpioRead(LED2) && current_time - led2_on_time >= led_on_duration)
            gpioWrite(LED2, 0);

        if (gpioRead(LED3) && current_time - led3_on_time >= led_on_duration)
            gpioWrite(LED3, 0);
    }


    printf("\nEjecutivo cíclico terminado.\n");
    return 0;
}