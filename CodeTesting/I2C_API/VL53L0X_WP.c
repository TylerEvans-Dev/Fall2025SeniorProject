#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#if defined(__APPLE__)
    #include "../GPIO/wiringOP/wiringPi/wiringPi.h"
    #include "../GPIO/wiringOP/wiringPi/wiringPiI2C.h"
#else
    #include <wiringPi.h>
    #include <wiringPiI2C.h>
#endif
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>





// Global file descriptor for I2C device
static int fd = -1;
static uint8_t current_addr = 0x29;  // default VL53L0X I2C address

// ------------------------------------------------------------
// Setup & basic I2C control
// ------------------------------------------------------------

// Start I2C communication with a given address
int i2c_start(uint8_t address)
{
    fd = wiringPiI2CSetup(address);
    if (fd < 0) {
        fprintf(stderr, "Error: failed to open I2C at 0x%02X\n", address);
        return -1;
    }
    current_addr = address;
    return 0;
}

// Repeated start condition — not needed on Linux, but we stub it
int i2c_rep_start(uint8_t address)
{
    // In Linux (WiringPi/WiringOP), the repeated start is automatically
    // handled by the kernel driver when doing a read after a write.
    // So this is just a no-op that returns success.
    (void)address;
    return 0;
}

// Stop condition — optional
void i2c_stop(void)
{
    // Nothing needed — kernel I2C driver handles this internally.
    // You can close() here if you want to explicitly end communication.
    if (fd >= 0) {
        // close(fd);  // Uncomment if you want to force close
        fd = -1;
    }
}

// ------------------------------------------------------------
// I2C data transfer functions
// ------------------------------------------------------------

// Write one byte (to the current I2C device)
int i2c_write(uint8_t data)
{
    if (fd < 0) {
        fprintf(stderr, "Error: I2C device not initialized!\n");
        return -1;
    }
    return wiringPiI2CWrite(fd, data);
}

// Read one byte and send ACK (for multi-byte reads)
// NOTE: WiringPiI2CRead() reads one byte and the kernel driver handles ACKs automatically.
uint8_t i2c_readAck(void)
{
    if (fd < 0) return 0;
    return (uint8_t)wiringPiI2CRead(fd);
}

// Read one byte and send NAK (end of read)
// NOTE: On Linux, there’s no distinction between readAck and readNak — kernel does it.
uint8_t i2c_readNak(void)
{
    if (fd < 0) return 0;
    return (uint8_t)wiringPiI2CRead(fd);
}

// ------------------------------------------------------------
// Register helpers (optional convenience functions)
// ------------------------------------------------------------

// Write one byte to a register
void i2c_writeReg(uint8_t reg, uint8_t value)
{
    if (fd >= 0)
        wiringPiI2CWriteReg8(fd, reg, value);
}

// Read one byte from a register
uint8_t i2c_readReg(uint8_t reg)
{
    if (fd < 0) return 0xFF;
    return (uint8_t)wiringPiI2CReadReg8(fd, reg);
}

// Multi-byte write
void i2c_writeMulti(uint8_t reg, const uint8_t *data, uint8_t count)
{
    if (fd < 0) return;
    for (uint8_t i = 0; i < count; i++)
        wiringPiI2CWriteReg8(fd, reg + i, data[i]);
}

// Multi-byte read
void i2c_readMulti(uint8_t reg, uint8_t *data, uint8_t count)
{
    if (fd < 0) return;
    for (uint8_t i = 0; i < count; i++)
        data[i] = (uint8_t)wiringPiI2CReadReg8(fd, reg + i);
}

// Optional function to get file descriptor (for advanced users)
int i2c_get_fd(void) { return fd; }
