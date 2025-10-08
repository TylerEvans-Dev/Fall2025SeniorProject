#include "../GPIO/wiringOP/wiringPi/wiringPi.h"
#include "../GPIO/wiringOP/wiringPi/wiringPiI2C.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define VL53L0X_ADDR 0x29

static inline void write8(int fd, uint8_t reg, uint8_t val) {
    wiringPiI2CWriteReg8(fd, reg, val);
}

static inline uint8_t read8(int fd, uint8_t reg) {
    return wiringPiI2CReadReg8(fd, reg);
}

// More comprehensive initialization
int vl53l0x_init(int fd) {
    // Power on sequence
    write8(fd, 0x88, 0x00);
    write8(fd, 0x80, 0x01);
    write8(fd, 0xFF, 0x01);
    write8(fd, 0x00, 0x00);

    // Stop continuous mode if running
    write8(fd, 0x91, 0x3C);
    write8(fd, 0x00, 0x01);
    write8(fd, 0xFF, 0x00);
    write8(fd, 0x80, 0x00);

    // Additional initialization steps
    write8(fd, 0x80, 0x01);
    write8(fd, 0xFF, 0x01);
    write8(fd, 0x00, 0x00);
    write8(fd, 0xFF, 0x00);
    write8(fd, 0x80, 0x00);

    return 0;
}

uint16_t read_distance(int fd) {
    // Start single measurement
    write8(fd, 0x80, 0x01);
    write8(fd, 0xFF, 0x01);
    write8(fd, 0x00, 0x00);
    write8(fd, 0x91, 0x3C);
    write8(fd, 0x00, 0x01);
    write8(fd, 0xFF, 0x00);
    write8(fd, 0x80, 0x00);

    // Start ranging
    write8(fd, 0x00, 0x01);

    // Wait for measurement
    uint8_t status = 0;
    int attempts = 0;
    do {
        status = read8(fd, 0x13);
        delay(10);
        attempts++;
        if (attempts > 500) { // Increased timeout
            printf("Timeout!\n");
            return 0xFFFF; // Error value
        }
    } while ((status & 0x07) == 0);

    // Read distance
    uint8_t high = read8(fd, 0x1E);
    uint8_t low = read8(fd, 0x1F);
    uint16_t distance = ((uint16_t)high << 8) | low;

    // Clear interrupt immediately
    write8(fd, 0x0B, 0x01);

    return distance;
}

int main(void) {
    printf("VL53L0X improved test\n");

    wiringPiSetup();
    int fd = wiringPiI2CSetup(VL53L0X_ADDR);
    if (fd < 0) {
        perror("I2C setup failed");
        return 1;
    }

    // Initialize sensor
    if (vl53l0x_init(fd) != 0) {
        printf("Init failed\n");
        return 1;
    }

    // Take multiple measurements
    for (int i = 0; i < 10; i++) {
        uint16_t distance = read_distance(fd);

        if (distance == 0xFFFF) {
            printf("Measurement %d: Error\n", i+1);
        } else {
            printf("Measurement %d: %u mm\n", i+1, distance);
        }

        delay(100); // Wait between measurements
    }

    return 0;
}
