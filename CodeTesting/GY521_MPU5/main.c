#if defined(__APPLE__)
#include "../GPIO/wiringOP/wiringPi/wiringPi.h"
#include "../GPIO/wiringOP/wiringPi/wiringPiI2C.h"
#else
#include <wiringPi.c>
#include <wiringPiI2C.h>
#endif
#include <stdint.h>
#include <stdio.h>
#include "GY521.h"

int main(){
    int q = wiringPiSetup();
    if (q < 0){
        printf("something has gone wrong with orange setup\n");
        return -1;
    }
    int suc = initGY521("/dev/i2c-4");


    return 0;
}
