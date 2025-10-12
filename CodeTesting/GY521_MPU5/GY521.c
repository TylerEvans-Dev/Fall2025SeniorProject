#include "GY521.h"
#include <stdint.h>

#define ADDR 0x68

int initGY521(const char *chan){
    fd = wiringPiI2CSetupInterface(chan, ADDR);
    if(fd < 0)
    {
        return -1;
    }
    uint8_t res = wiringPiI2CReadReg8(fd, 0x75);
    if(res != ADDR){
        printf("The device is not found please try again fool\n");
        printf("the device read %d or %n \n", res, res);
    }
    printf("success :) the value red from the device is %d \n", res);
    return 0;
}
