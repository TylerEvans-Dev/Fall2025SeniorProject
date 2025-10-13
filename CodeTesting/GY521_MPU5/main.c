#if defined(__APPLE__)
#include "../GPIO/wiringOP/wiringPi/wiringPi.h"
#include "../GPIO/wiringOP/wiringPi/wiringPiI2C.h"
#else
#include <wiringPi.h>
#include <wiringPiI2C.h>
#endif
#include <stdint.h>
#include <stdio.h>
#include "GY521.h"
int main(){
    int q = wiringPiSetup();
    DataAccel d;
    if (q < 0){
        printf("something has gone wrong with orange setup\n");
        return -1;
    }
    int suc = initGY521("/dev/i2c-4");
    int readsuc = mpu6050_read_all(fd, &d);
    printf("A: %.2f %.2f %.2f | G: %.2f %.2f %.2f | T: %.2f°C\n",
                   d.ax, d.ay, d.az, d.gx, d.gy, d.gz, d.temp);
    delay(20);
    return 0;
}
