#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pigpio.h>
#include <unistd.h>

#define SERVO_PIN 18
#define MPU6050_ADDR 0x68
#define I2C_BUS 1

// Registros MPU6050
#define PWR_MGMT_1 0x6B
#define ACCEL_XOUT_H 0x3B
#define ACCEL_CONFIG 0x1C

// Configuración para el acelerómetro (±2g)
#define ACCEL_FS_SEL_2G 0x00
#define ACCEL_SCALE 16384.0 // Escala para ±2g (LSB/g)

void mpu6050_init(int fd) {
    // Despertar el MPU6050
    i2cWriteByteData(fd, PWR_MGMT_1, 0x00);
    // Configurar el rango del acelerómetro a ±2g
    i2cWriteByteData(fd, ACCEL_CONFIG, ACCEL_FS_SEL_2G);
}

int16_t read_accel_axis(int fd, uint8_t reg) {
    uint8_t buf[2];
    i2cReadI2CBlockData(fd, reg, buf, 2);
    return (int16_t)((buf[0] << 8) | buf[1]);
}

double calculate_angle(double accel_x, double accel_z) {
    // Calcular ángulo en radianes usando atan2 y convertir a grados
    double angle_rad = atan2(accel_x, accel_z);
    double angle_deg = angle_rad * 180.0 / M_PI;

    // Escalar de -90° a +90° a 0° a 180° para el servo
    return angle_deg + 90.0;
}

int main() {
    // Inicializar pigpio
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Error al inicializar pigpio\n");
        return 1;
    }

    // Inicializar I2C
    int fd = i2cOpen(I2C_BUS, MPU6050_ADDR, 0);
    if (fd < 0) {
        fprintf(stderr, "Error al abrir el bus I2C\n");
        gpioTerminate();
        return 1;
    }

    // Configurar MPU6050
    mpu6050_init(fd);

    printf("Control de Servo SG90 con MPU6050\n");
    printf("Leyendo datos del acelerómetro y controlando servo...\n");

    while (1) {
        // Leer valores del acelerómetro
        int16_t accel_x = read_accel_axis(fd, ACCEL_XOUT_H);
        int16_t accel_z = read_accel_axis(fd, ACCEL_XOUT_H + 4); // Z está en 0x3F

        // Convertir a valores en g (considerando ±2g)
        double accel_x_g = accel_x / ACCEL_SCALE;
        double accel_z_g = accel_z / ACCEL_SCALE;

        // Calcular ángulo de inclinación
        double angle = calculate_angle(accel_x_g, accel_z_g);

        // Asegurar que el ángulo esté en el rango 0-180
        if (angle < 0) angle = 0;
        if (angle > 180) angle = 180;

        // Convertir ángulo a microsegundos (500-2500μs)
        int pulsewidth = 500 + (int)(angle * 2000 / 180);

        // Enviar señal PWM al servo
        gpioServo(SERVO_PIN, pulsewidth);

        // Mostrar información
        printf("Ángulo: %.1f° | Pulso: %d μs | Aceleración X: %.2fg Z: %.2fg",
               angle, pulsewidth, accel_x_g, accel_z_g);

        // Esperar 10ms antes de la siguiente actualización
        usleep(10000);
    }

    // Limpiar (este código normalmente no se alcanza debido al bucle infinito)
    i2cClose(fd);
    gpioServo(SERVO_PIN, 0);
    gpioTerminate();

    return 0;
}