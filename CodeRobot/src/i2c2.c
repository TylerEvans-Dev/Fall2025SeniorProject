#include <stdio.h>
#include "../wiringOP/wiringPi/wiringPi.h"
#include "../wiringOP/wiringPi/wiringPiI2C.h"

//stiar
int main(){
    //printf("watermelon sugar\n");
    //printf("chicken strips\n");
    int s = wiringPiSetup();
    int fd;
    if(!s){
        printf("failed to setup value of s is %i \n", s);
        return -1;
    }
    printf("I2C Init\n");
    int reg = 0x29;
    fd = wiringPiI2CSetup(fd);
    if(fd == -1){
        printf("failure to setup I2C device\n");
    }
    //this is reading the ISR on reg 16.
    int res = wiringPiI2CReadReg16(fd, reg);
    if(res == -1){
        printf("Failed to find devic. ");
        return -1;
    }
    else{
         printf("Register 0x%X: 0x%X\n", reg, res);
    }

    return 0;
}
