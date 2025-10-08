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

int main(void) {
    printf("VL53L0X Basic Communication Test\n");

    wiringPiSetup();
    int fd = wiringPiI2CSetup(VL53L0X_ADDR);
    if (fd < 0) {
        perror("I2C setup failed");
        return 1;
    }
    printf("I2C file descriptor: %d\n", fd);

    // Test 1: Read WHO_AM_I register (should be 0xEE for VL53L0X)
    uint8_t whoami = read8(fd, 0xC0);
    printf("WHO_AM_I register (0xC0): 0x%02X\n", whoami);

    // Test 2: Read model ID register
    uint8_t model_id = read8(fd, 0xC1);
    printf("Model ID register (0xC1): 0x%02X\n", model_id);

    // Test 3: Try to read a few more registers
    for (int i = 0; i < 5; i++) {
        uint8_t val = read8(fd, 0x10 + i);
        printf("Register 0x%02X: 0x%02X\n", 0x10 + i, val);
    }

    return 0;
}
