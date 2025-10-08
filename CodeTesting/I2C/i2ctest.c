#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define VL53L0X_ADDR 0x29

// ---------- Basic I2C helpers ----------
void i2c_write_byte(int fd, uint8_t reg, uint8_t val) {
    wiringPiI2CWriteReg8(fd, reg, val);
}

uint8_t i2c_read_byte(int fd, uint8_t reg) {
    return wiringPiI2CReadReg8(fd, reg);
}

uint16_t i2c_read_word(int fd, uint8_t reg) {
    int hi = wiringPiI2CReadReg8(fd, reg);
    int lo = wiringPiI2CReadReg8(fd, reg + 1);
    return (hi << 8) | lo;
}

// ---------- Wait for measurement completion ----------
void wait_for_measurement(int fd) {
    uint8_t status;
    do {
        status = i2c_read_byte(fd, 0x13);
        delay(10);
    } while ((status & 0x07) == 0);
}

// ---------- VL53L0X minimal init ----------
void vl53l0x_init(int fd) {
    // Power-up and boot sequence
    i2c_write_byte(fd, 0x88, 0x00);
    i2c_write_byte(fd, 0x80, 0x01);
    i2c_write_byte(fd, 0xFF, 0x01);
    i2c_write_byte(fd, 0x00, 0x00);
    i2c_write_byte(fd, 0x91, 0x3C);
    i2c_write_byte(fd, 0x00, 0x01);
    i2c_write_byte(fd, 0xFF, 0x00);
    i2c_write_byte(fd, 0x80, 0x00);

    // Minimal SPAD & sequence config
    i2c_write_byte(fd, 0x01, 0x02);
}

// ---------- Take one distance reading ----------
uint16_t vl53l0x_read_distance(int fd) {
    // Start single measurement
    i2c_write_byte(fd, 0x00, 0x01);

    // Wait until data ready
    wait_for_measurement(fd);

    // Read distance result (register 0x1E or 0x14 depending on revision)
    uint16_t dist = i2c_read_word(fd, 0x1E);

    // Clear interrupt
    i2c_write_byte(fd, 0x0B, 0x01);

    return dist;
}

// ---------- Main ----------
int main(void) {
    printf("VL53L0X Test (WiringOP)\n");

    int fd = wiringPiI2CSetup(VL53L0X_ADDR);
    if (fd < 0) {
        printf("Failed to init I2C.\n");
        return -1;
    }

    vl53l0x_init(fd);

    while (1) {
        uint16_t dist = vl53l0x_read_distance(fd);
        printf("Distance: %d mm\n", dist);
        delay(500);
    }

    return 0;
}

