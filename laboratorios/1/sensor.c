#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pigpio.h>

#define AHT10_I2C_ADDR 0x38
#define I2C_BUS 1

#define TEMP_THRESHOLD 25.0
#define HUMIDITY_THRESHOLD 70.0

#define TEMP_GPIO_PIN 17
#define HUMIDITY_GPIO_PIN 27

int i2c_handle;

// Función para inicializar el sensor AHT10
int initialize_aht10() {
    char init_cmd[3] = {0xE1, 0x08, 0x00};

    // Enviar comando de inicialización
    if (i2cWriteDevice(i2c_handle, init_cmd, 3) != 0) {
        fprintf(stderr, "Error al inicializar el sensor AHT10\n");
        return -1;
    }

    // Esperar a que el sensor se inicialice
    usleep(10000); // 10ms

    return 0;
}

// Función para leer temperatura y humedad del sensor
int read_aht10(float *temperature, float *humidity) {
    char trigger_cmd[3] = {0xAC, 0x33, 0x00};
    char data[6] = {0};

    // Enviar comando de medición
    if (i2cWriteDevice(i2c_handle, trigger_cmd, 3) != 0) {
        fprintf(stderr, "Error al enviar comando de medición\n");
        return -1;
    }

    // Esperar a que la medición esté lista
    usleep(80000); // 80ms

    // Leer datos del sensor
    if (i2cReadDevice(i2c_handle, data, 6) != 6) {
        fprintf(stderr, "Error al leer datos del sensor\n");
        return -1;
    }

    // Verificar si la medición está lista (bit 7 del primer byte)
    if (data[0] & 0x80) {
        fprintf(stderr, "Datos no listos\n");
        return -1;
    }

    // Procesar datos de humedad (20 bits)
    unsigned int humidity_raw = ((unsigned int)data[1] << 12) |
                               ((unsigned int)data[2] << 4) |
                               ((unsigned int)data[3] >> 4);
    *humidity = (float)humidity_raw * 100.0 / (1 << 20);

    // Procesar datos de temperatura (20 bits)
    unsigned int temperature_raw = (((unsigned int)data[3] & 0x0F) << 16) |
                                  ((unsigned int)data[4] << 8) |
                                  (unsigned int)data[5];
    *temperature = (float)temperature_raw * 200.0 / (1 << 20) - 50.0;

    return 0;
}

// Función para controlar las salidas GPIO según los umbrales
void control_outputs(float temperature, float humidity) {
    // Controlar salida de temperatura
    if (temperature > TEMP_THRESHOLD) {
        gpioWrite(TEMP_GPIO_PIN, 1);
        printf("Temperatura ALTA - GPIO %d ACTIVADO\n", TEMP_GPIO_PIN);
    } else {
        gpioWrite(TEMP_GPIO_PIN, 0);
    }

    // Controlar salida de humedad
    if (humidity > HUMIDITY_THRESHOLD) {
        gpioWrite(HUMIDITY_GPIO_PIN, 1);
        printf("Humedad ALTA - GPIO %d ACTIVADO\n", HUMIDITY_GPIO_PIN);
    } else {
        gpioWrite(HUMIDITY_GPIO_PIN, 0);
    }
}

int main() {
    float temperature = 0.0;
    float humidity = 0.0;

    // Inicializar pigpio
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Error al inicializar pigpio\n");
        return 1;
    }

    // Configurar pines GPIO como salidas
    gpioSetMode(TEMP_GPIO_PIN, PI_OUTPUT);
    gpioSetMode(HUMIDITY_GPIO_PIN, PI_OUTPUT);

    // Inicializar comunicación I2C
    i2c_handle = i2cOpen(I2C_BUS, AHT10_I2C_ADDR, 0);
    if (i2c_handle < 0) {
        fprintf(stderr, "Error al abrir comunicación I2C\n");
        gpioTerminate();
        return 1;
    }

    // Inicializar sensor
    if (initialize_aht10() != 0) {
        fprintf(stderr, "No se pudo inicializar el sensor\n");
        i2cClose(i2c_handle);
        gpioTerminate();
        return 1;
    }

    printf("Sensor AHT10 inicializado correctamente\n");

    // Bucle principal
    while (1) {
        if (read_aht10(&temperature, &humidity) == 0) {
            printf("Temperatura: %.2f °C, Humedad: %.2f%%\n", temperature, humidity);
            control_outputs(temperature, humidity);
        }

        sleep(1); // Esperar 1 segundo entre lecturas
    }

    i2cClose(i2c_handle);
    gpioTerminate();
    return 0;
}