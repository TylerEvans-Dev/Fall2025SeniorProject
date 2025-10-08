#include <stdio.h>
#include "../wiringOP/wiringPi/wiringPi.h"
#include "../wiringOP/wiringPi/wiringPiI2C.h"

int main(void) {
    int fd;
    int deviceAddr = 0x29;  // replace with your device address
    int reg = 0x00;         // replace with your register
    int data;

    // Setup wiringPi
    if (wiringPiSetup() == -1) {
        printf("wiringPi setup failed!\n");
        return 1;
    }

    // Open I2C device
    fd = wiringPiI2CSetup(deviceAddr);
    if (fd == -1) {
        printf("Failed to init I2C communication.\n");
        return 1;
    }
    printf("I2C communication successfully setup.\n");

    // Read data from a register
    data = wiringPiI2CReadReg8(fd, reg);
    if (data == -1) {
        printf("Failed to read from I2C device.\n");
    } else {
        printf("Data read from reg 0x%02X: 0x%02X\n", reg, data);
    }

    // Write data to a register
    // if (wiringPiI2CWriteReg8(fd, reg, 0x12) == -1) {
    //     printf("Failed to write to I2C device.\n");
    // } else {
    //     printf("Wrote 0x12 to reg 0x%02X\n", reg);
    // }

    return reg;
}
