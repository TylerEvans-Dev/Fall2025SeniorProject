#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define VL53L0X_ADDR 0x29

// Simple read/write helpers
static inline void write8(int fd, uint16_t reg, uint8_t val) {
    wiringPiI2CWriteReg8(fd, reg, val);
}

static inline uint8_t read8(int fd, uint16_t reg) {
    return wiringPiI2CReadReg8(fd, reg);
}

static inline uint16_t read16(int fd, uint16_t reg) {
    uint8_t high = read8(fd, reg);
    uint8_t low  = read8(fd, reg + 1);
    return (high << 8) | low;
}

int main(void) {
    printf("VL53L0X Minimal Ranging Example\n");

    wiringPiSetup();
    int fd = wiringPiI2CSetup(VL53L0X_ADDR);
    if (fd < 0) {
        perror("I2C setup failed");
        return 1;
    }

    printf("I2C FD: %d\n", fd);

    // ===== Minimal initialization =====
    // Step 1: Perform software reset (optional, depends on module)
    write8(fd, 0x000, 0x01);
    usleep(1000);  // wait 1ms

    // Step 2: Set up sensor (simplified default settings)
    write8(fd, 0x88, 0x00);
    write8(fd, 0x80, 0x01);
    write8(fd, 0xFF, 0x01);
    write8(fd, 0x00, 0x00);

    // Step 3: Start continuous measurement
    write8(fd, 0x00, 0x01);  // Start ranging
    usleep(50000);            // wait 50ms for first measurement

    // ===== Main loop: read distance =====
    while (1) {
        // Distance register is 0x1E (high byte) + 0x1F (low byte)
        uint16_t distance = read16(fd, 0x1E);
        printf("Distance: %d mm\n", distance);

        usleep(50000);  // 50ms between reads
    }

    return 0;
}
