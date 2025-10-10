#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h> // For sleep/usleep

#define ADR 0x29

// Function to perform an I2C write and check the result
int safe_i2c_write(int fd, int reg, uint8_t value) {
    int status = wiringPiI2CWriteReg8(fd, reg, value);
    if (status == -1) {
        printf("I2C WRITE FAILED for Reg 0x%X (Val 0x%X)\n", reg, value);
    }
    return status;
}

int main() {
    // ... (Setup checks for wiringPiSetup and wiringPiI2CSetup - assuming successful) ...

    int fdq = wiringPiI2CSetup(ADR);
    if (fdq < 0) {
        printf("ERROR: Device setup failed.\n");
        return 1;
    }
    printf("Device setup successful (fdq: %d)\n", fdq);

    // --- VL53L0X STARTUP SEQUENCE (Mandatory) ---
    printf("Starting VL53L0X initialization writes...\n");

    // 1. Reset / Enable Sequence (Often needed for proper startup)
    // The VL53L0X powers up in 1.8V mode, these writes move it to a known state.
    if (safe_i2c_write(fdq, 0x88, 0x00) == -1) return 1;
    if (safe_i2c_write(fdq, 0x80, 0x01) == -1) return 1;
    if (safe_i2c_write(fdq, 0xFF, 0x01) == -1) return 1;
    if (safe_i2c_write(fdq, 0x00, 0x00) == -1) return 1;
    if (safe_i2c_write(fdq, 0x91, 0x3c) == -1) return 1;
    if (safe_i2c_write(fdq, 0x00, 0x01) == -1) return 1;
    if (safe_i2c_write(fdq, 0xFF, 0x00) == -1) return 1;

    usleep(10000); // Wait a short time

    printf("Initialization writes complete. Attempting to read Model ID (0xC0).\n");

    // 2. Read the Model ID (Expected: 0xEE or 0xAA for VL53L0X)
    int ReadResult = wiringPiI2CReadReg8(fdq, 0xC0);

    if (ReadResult == -1) {
        printf("FINAL ERROR: Read failed even after initialization. Check XSHUT/VCC.\n");
    } else {
        uint8_t ReadVal = (uint8_t)ReadResult;
        printf("SUCCESS: Read 8-bit reg val from 0xC0: 0x%X (%u)\n", ReadVal, ReadVal);

        if (ReadVal == 0xEE || ReadVal == 0xAA) {
            printf("Confirmed Device ID. Communication is working!\n");
        } else {
            printf("Unexpected Device ID (0x%X). Wiring is OK, but sequence may be wrong.\n", ReadVal);
        }
    }

    return 0;
}
