#ifndef VLX_H
#define VLX_H
#include <cstdint>
#include <stdint.h>


//this is polling I2C

typedef enum{
    I2C_OK,
    I2C_ERROR_STP,
    I2C_ERROR_TX,
    I2C_ERROR_RX,
    I2C_ERROR_STOP,
    I2C_ERROR_TIME_OUT
} i2c_res;

//inits the VLX
//
void i2c_init(uint8_t addr);
void i2c_SET_ADDR(uint8_t addr);
i2c_res i2c_write(const uint8_t *addr, uint8_t addr_size, const uint8_t *data, uint8_t datasize);
i2c_res i2c_read(const uint8_t *addr, uint8_t addr_size, uint8_t *data, uint8_t datasize);




#endif
