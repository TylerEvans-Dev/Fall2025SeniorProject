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

#define ADR 0x29
//this is a basic setup of the VLX code and I hope this works =)
int main(){
    //these are two checks on the setup of the wiring OP
    int fd;
    fd = wiringPiSetup();
    if(fd < 0){
        printf("failed to setup the wiringOP\n");
    }
    printf("setup wiringOP\n");
    int fdq = wiringPiI2CSetup(ADR);
    if(fdq < 0){
        printf("failed to get device from addr\n");
    }
    printf("device setup \n");

    uint8_t ReadVal = wiringPiI2CReadReg8(fdq, 0xC0);
    printf("the 8 reg val %d \n", ReadVal);
    uint16_t ReadVal1 = wiringPiI2CReadReg16(fdq, 0xC0);
    printf("the 16 reg val %d \n", ReadVal1);
    int ReadVal3 = wiringPiI2CRead(fdq);
    printf("the reg val %d \n", ReadVal3);
    return 0;





}
