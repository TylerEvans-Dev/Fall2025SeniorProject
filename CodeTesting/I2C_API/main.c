#include "../GPIO/wiringOP/wiringPi/wiringPi.h"
#include "../GPIO/wiringOP/wiringPi/wiringPiI2C.h"
//#include "wiringPi.h"
// #include "wiringPiI2C.h"
#include <stdint.h>
#include <stdio.h>

#include <unistd.h>

// VL53L0X default I2C address
#define VL53L0X_ADDR 0x29

// Helper for writing and reading
static inline void write8(int fd, uint8_t reg, uint8_t val) {
    wiringPiI2CWriteReg8(fd, reg, val);
}

static inline uint8_t read8(int fd, uint8_t reg) {
    return wiringPiI2CReadReg8(fd, reg);
}

int main(void) {
    printf("VL53L0X minimal init test\n");

    wiringPiSetup();
    int fd = wiringPiI2CSetup(VL53L0X_ADDR);
    if (fd < 0) {
        perror("I2C setup failed");
        return 1;
    }

    // === 1. Sensor boot sequence ===
    write8(fd, 0x88, 0x00);
    write8(fd, 0x80, 0x01);
    write8(fd, 0xFF, 0x01);
    write8(fd, 0x00, 0x00);
    write8(fd, 0x91, 0x3C);  // typical calibration value
    write8(fd, 0x00, 0x01);
    write8(fd, 0xFF, 0x00);
    write8(fd, 0x80, 0x00);

    // === 2. Start single-ranging measurement ===
    write8(fd, 0x00, 0x01);  // "system fresh out of reset"
    write8(fd, 0x80, 0x01);
    write8(fd, 0xFF, 0x01);
    write8(fd, 0x00, 0x00);
    write8(fd, 0x91, 0x3C);
    write8(fd, 0x00, 0x01);
    write8(fd, 0xFF, 0x00);
    write8(fd, 0x80, 0x00);

    // Begin measurement
    write8(fd, 0x00, 0x01);
    write8(fd, 0x00, 0x03); // start ranging

    // === 3. Wait until measurement ready ===
    uint8_t status = 0;
    int attempts = 0;
    do {
        status = read8(fd, 0x13); // result interrupt status
        delay(10);
        attempts++;
        if (attempts > 200) {
            printf("Timeout waiting for measurement ready.\n");
            return 1;
        }
    } while ((status & 0x07) == 0);  // wait until ready

    // === 4. Read distance result ===
    uint8_t high = read8(fd, 0x1E);
    uint8_t low  = read8(fd, 0x1F);
    uint16_t distance_mm = ((uint16_t)high << 8) | low;

    printf("Measured distance: %u mm\n", distance_mm);

    // Clear interrupt
    write8(fd, 0x0B, 0x01);

    return 0;
}
