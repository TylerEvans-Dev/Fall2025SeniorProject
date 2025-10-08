#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define VL53L0X_ADDR 0x29

uint8_t read_byte(int fd, uint8_t reg) {
    return wiringPiI2CReadReg8(fd, reg);
}

void write_byte(int fd, uint8_t reg, uint8_t val) {
    wiringPiI2CWriteReg8(fd, reg, val);
}

uint16_t read_word(int fd, uint8_t reg) {
    int hi = wiringPiI2CReadReg8(fd, reg);
    int lo = wiringPiI2CReadReg8(fd, reg + 1);
    return (hi << 8) | lo;
}

void vl53l0x_init(int fd) {
    delay(10);
    write_byte(fd, 0x88, 0x00);
    write_byte(fd, 0x80, 0x01);
    write_byte(fd, 0xFF, 0x01);
    write_byte(fd, 0x00, 0x00);
    write_byte(fd, 0x91, 0x3C);
    write_byte(fd, 0x00, 0x01);
    write_byte(fd, 0xFF, 0x00);
    write_byte(fd, 0x80, 0x00);

    // System sequence config
    write_byte(fd, 0x01, 0x02);

    // Start continuous ranging
    write_byte(fd, 0x80, 0x01);
    write_byte(fd, 0xFF, 0x01);
    write_byte(fd, 0x00, 0x00);
    write_byte(fd, 0x91, 0x3C);
    write_byte(fd, 0x00, 0x01);
    write_byte(fd, 0xFF, 0x00);
    write_byte(fd, 0x80, 0x00);

    write_byte(fd, 0x00, 0x04); // start continuous mode
}

uint16_t vl53l0x_read_distance(int fd) {
    uint16_t dist = read_word(fd, 0x14);
    return dist;
}

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
