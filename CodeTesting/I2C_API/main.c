#include "../GPIO/wiringOP/wiringPi/wiringPi.h"
#include "../GPIO/wiringOP/wiringPi/wiringPiI2C.h"
//#include "wiringPi.h"
// #include "wiringPiI2C.h"
#include <stdint.h>
#include <stdio.h>

int main(void) {
    printf("Start of the I2C test program\n");

    if (wiringPiSetup() == -1) {
        perror("WiringOP setup failed");
        return 1;
    }

    int fd = wiringPiI2CSetup(0x29);  // VL53L0X default address
    if (fd < 0) {
        perror("Failed to open I2C device");
        return 1;
    }

    int reg = 0x00;  // first register (safe to read)
    int val8 = wiringPiI2CReadReg8(fd, reg);
    printf("8-bit read: reg 0x%02X -> 0x%02X\n", reg, val8 & 0xFF);

    int val16 = wiringPiI2CReadReg16(fd, reg);
    printf("16-bit read: reg 0x%02X -> 0x%04X\n", reg, val16 & 0xFFFF);

    return 0;
}
