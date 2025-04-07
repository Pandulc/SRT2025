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

// Prototipos de funciones
void handle_signal(int sig);
void print_elapsed_time(const char* task_name);
void task1(void);
void task2(void);
void task3(void);
void init_executive(void);
double get_elapsed_time(void);

// Manejador de señal para Ctrl+C
void handle_signal(int sig) {
    keep_running = 0;
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
void task1(void) {
    print_elapsed_time("Tarea 1 (100 ms)");
    gpioWrite(LED1, 1);
    usleep(90);
    gpioWrite(LED1, 0);
}

void task2(void) {
    print_elapsed_time("Tarea 2 (300 ms)");
    gpioWrite(LED2, 1);
    usleep(90);
    gpioWrite(LED2, 0);
}

void task3(void) {
    print_elapsed_time("Tarea 3 (500 ms)");
    gpioWrite(LED3, 1);
    usleep(90);
    gpioWrite(LED3, 0);
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

    // Variables para el control de los períodos
    unsigned long next_100ms = 0;
    unsigned long next_300ms = 0;
    unsigned long next_500ms = 0;

    // Bucle principal del ejecutivo cíclico
    while (keep_running) {
        // Obtener el tiempo actual en milisegundos
        unsigned long current_time = (unsigned long)(get_elapsed_time() * 1000);

        // Ejecutar tarea 1 cada 100 ms
        if (current_time >= next_100ms) {
            task1();
            next_100ms = current_time + 100;
        }

        // Ejecutar tarea 2 cada 300 ms
        if (current_time >= next_300ms) {
            task2();
            next_300ms = current_time + 300;
        }

        // Ejecutar tarea 3 cada 500 ms
        if (current_time >= next_500ms) {
            task3();
            next_500ms = current_time + 500;
        }
    }

    printf("\nEjecutivo cíclico terminado.\n");
    return 0;
}