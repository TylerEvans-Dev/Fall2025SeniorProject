#include "VLX.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#if defined(__APPLE__)
#include "wiringOP/wiringPi/wiringPi.h"
#include "wiringOP/wiringPi/wiringPiI2C.h"
#else
#include <wiringPi.h>
#include <wiringPiI2C.h>
#endif

static bool init = 0;
static int fd = 0;
void i2c_init(uint8_t addr){
    fd = wiringPiSetup();
    if(fd < 0){
        printf("failed to setup the wiringOP");
    }
}


void i2c_SET_ADDR(uint8_t addr){

}
i2c_res i2c_write(const uint8_t *addr, uint8_t addr_size, const uint8_t *data, uint8_t datasize){

}
i2c_res i2c_read(const uint8_t *addr, uint8_t addr_size, uint8_t *data, uint8_t datasize){

}

//TODO Wrapper functions
i2c_res i2cRead_8_D_8(uint8_t addr, uint8_t, uint8_t *data){

}
i2c_res i2cRead_8_D_16(uint8_t addr, uint8_t, uint16_t *data){

}
i2c_res i2cWrite_8_D_32(uint8_t addr, uint8_t, uint32_t *data){

}
i2c_res i2cWrite_8_D_16(uint8_t addr, uint8_t, uint16_t *data){

}
