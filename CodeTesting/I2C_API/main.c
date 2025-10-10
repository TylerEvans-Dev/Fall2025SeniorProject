
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

#if defined(__APPLE__)
    #include "../GPIO/wiringOP/wiringPi/wiringPi.h"
    #include "../GPIO/wiringOP/wiringPi/wiringPiI2C.h"
#else
    #include <wiringPi.h>
    #include <wiringPiI2C.h>
#endif

#include "VL53L0X.h"

// The VL53L0X default I2C address
#define VL53L0X_ADDR 0x29

int main(void) {
    printf("=== VL53L0X Distance Sensor Test ===\n");

    // Initialize WiringPi
    if (wiringPiSetupSys() == -1) {
        fprintf(stderr, "Failed to initialize WiringPi\n");
        return 1;
    }

    // Initialize I2C for the VL53L0X
    initVL53L0X(VL53L0X_ADDR);

    // Initialize the sensor
    printf("Initializing sensor...\n");
    if (!initVL53L0X(true)) {
        fprintf(stderr, "VL53L0X initialization failed!\n");
        return 1;
    }

    // Optional: set measurement timing budget (e.g. 33ms default)
    setMeasurementTimingBudget(33000);

    printf("Sensor initialized successfully.\n\n");

    // Main loop: continuously read distance
    while (1) {
        uint16_t distance = readRangeSingleMillimeters(NULL);

        if (timeoutOccurred()) {
            printf("Timeout occurred!\n");
        } else if (distance == 65535) {
            printf("Invalid measurement (out of range)\n");
        } else {
            printf("Distance: %u mm\n", distance);
        }

        delay(200); // wait 200 ms between readings
    }

    return 0;
}
