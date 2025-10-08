#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define VL53L0X_ADDRESS 0x29
#define XSHUT_PIN 4  // WiringPi pin connected to XSHUT

int fd;

// I2C write helper
void writeReg(uint8_t reg, uint8_t value) {
    wiringPiI2CWriteReg8(fd, reg, value);
}

// I2C read helper
uint8_t readReg(uint8_t reg) {
    return wiringPiI2CReadReg8(fd, reg);
}

// Reset sensor via XSHUT
void sensorReset() {
    pinMode(XSHUT_PIN, OUTPUT);
    digitalWrite(XSHUT_PIN, LOW);
    usleep(10000);  // 10 ms
    digitalWrite(XSHUT_PIN, HIGH);
    usleep(10000);  // 10 ms
}

// Minimal initialization
int initVL53L0X() {
    // Check device ID
    uint8_t model_id = readReg(0xC0);
    if (model_id != 0xEE) {
        printf("VL53L0X not found! ID = 0x%X\n", model_id);
        return -1;
    }

    // 1. Stop any measurement
    writeReg(0x00, 0x01);

    // 2. Load default VHV config (from datasheet)
    writeReg(0x89, 0x01);
    writeReg(0xFF, 0x01);
    writeReg(0x00, 0x00);
    writeReg(0x91, readReg(0x91) & ~0x01); // clear bit
    writeReg(0x00, 0x01);
    writeReg(0xFF, 0x00);
    writeReg(0x89, 0x00);

    // 3. Perform reference SPAD management
    writeReg(0x80, 0x01);
    writeReg(0xFF, 0x01);
    writeReg(0x00, 0x00);
    writeReg(0xFF, 0x00);
    writeReg(0x80, 0x00);

    // 4. Perform calibration
    writeReg(0xE0, 0x01); // enable VHV calibration
    usleep(1000);          // wait a bit
    writeReg(0xE0, 0x00);

    printf("VL53L0X initialized successfully!\n");
    return 0;
}

// Start continuous measurements
void startContinuous(uint16_t period_ms) {
    writeReg(0x80, 0x01);
    writeReg(0xFF, 0x01);
    writeReg(0x00, 0x00);
    writeReg(0x91, 0x00);
    writeReg(0x00, 0x01);
    writeReg(0xFF, 0x00);
    writeReg(0x80, 0x00);

    // Start continuous measurement
    writeReg(0x00, 0x04); // SYSRANGE_START register, continuous mode
    usleep(period_ms * 1000);
}

int main() {
    if (wiringPiSetup() == -1) {
        printf("WiringPi setup failed\n");
        return 1;
    }

    fd = wiringPiI2CSetup(VL53L0X_ADDRESS);
    if (fd == -1) {
        printf("Failed to init I2C\n");
        return 1;
    }

    sensorReset();

    if (initVL53L0X() != 0) {
        return 1;
    }

    // Start continuous measurement every 50ms
    startContinuous(50);

    while (1) {
        // Read distance from RESULT_RANGE_STATUS (0x14..0x15)
        uint8_t high = readReg(0x1E); // high byte
        uint8_t low  = readReg(0x1F); // low byte
        int distance = (high << 8) | low;
        printf("Distance: %d mm\n", distance);
        usleep(50000);
    }

    return 0;
}

