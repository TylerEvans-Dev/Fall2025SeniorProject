
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include "VL53L0X.h"
#if defined(__APPLE__)
    #include "../GPIO/wiringOP/wiringPi/wiringPi.h"
    #include "../GPIO/wiringOP/wiringPi/wiringPiI2C.h"
#else
    #include <wiringPi.h>
    #include <wiringPiI2C.h>
#endif


int main() {
    wiringPiSetup();

    //VL53L0X_I2CInit();  // sets up I²C device using wiringPiI2CSetup()
    initVL53L0X(1);

    if (!initVL53L0X(true)) {
        printf("Failed to initialize VL53L0X\n");
        return 1;
    }

    while (1) {
        uint16_t distance = readRangeSingleMillimeters(NULL);
        if (timeoutOccurred()) {
            printf("Timeout!\n");
        } else {
            printf("Distance: %d mm\n", distance);
        }
        delay(100);
    }
}
