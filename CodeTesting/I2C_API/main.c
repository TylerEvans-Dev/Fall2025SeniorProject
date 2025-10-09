
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include "VL53L0X.h"
#include "VL53L0X_WP.c"
#if defined(__APPLE__)
    #include "../GPIO/wiringOP/wiringPi/wiringPi.h"
    #include "../GPIO/wiringOP/wiringPi/wiringPiI2C.h"
#else
    #include <wiringPi.h>
    #include <wiringPiI2C.h>
#endif


int main(void)
{
    if (i2c_start(0x29) < 0) {
        printf("Failed to start I2C.\n");
        return 1;
    }

    if (!initVL53L0X(true)) {
        printf("Sensor init failed.\n");
        return 1;
    }

    printf("VL53L0X initialized!\n");

    while (1) {
        uint16_t distance = readRangeSingleMillimeters(NULL);
        printf("Distance: %u mm\n", distance);
        usleep(200000);
    }

    return 0;
}
