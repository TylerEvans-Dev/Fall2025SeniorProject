#include "../GPIO/wiringOP/wiringPi/wiringPi.h"
#include "../GPIO/wiringOP/wiringPi/wiringPiI2C.h"
//#include "wiringPi.h"
// #include "wiringPiI2C.h"
#include <stdint.h>
#include <stdio.h>

void setup_init(){

}


int main(){
    int8_t reg = 0x00;
    int16_t reg1 = 0x00;
    //starting
    printf("start of the test  program for I2C");
    wiringPiSetup();

    int fd = wiringPiI2CSetup(0x29);
    if(fd != 1){
        printf("device did not connect :(\n");
    }
    int status = wiringPiI2CReadReg8(fd,reg);
    printf("the status is %d and the value is %d\n", status, reg);
    int status2 = wiringPiI2CReadReg16(fd, reg1);
    printf("the status is %d and the value is %d\n", status2, reg1);
    return 0;
}
