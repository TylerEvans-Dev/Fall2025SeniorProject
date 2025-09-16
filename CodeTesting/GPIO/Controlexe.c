#include <stdio.h>
//this is the wiring pi setup here.
#include <wiringPi.h>
#include <wiringPiI2C.h>

//pin def. for use on the Ornage pi 2w zero
// PWM pins
#define GPIO_PWM1 21
#define GPIO_PWM2 22
#define GPIO_PWM3 2
#define GPIO_PWM4 9
//This is the SDA and SCL pins
#define GPIO_I2C2_SDA 17
#define GPIO_I2C2_SCL 18
//device address of Infared Sensor
#define IFSADR 0x20

void pinSetup(){
    //sets up the pins for control of motors
    pinMode(GPIO_PWM1,PWM_OUTPUT);
    pinMode(GPIO_PWM2,PWM_OUTPUT);
    pinMode(GPIO_PWM3,PWM_OUTPUT);
    pinMode(GPIO_PWM4,PWM_OUTPUT);
    //setup to zero by default
    pwmWrite(GPIO_PWM1, 0);
    pwmWrite(GPIO_PWM2, 0);
    pwmWrite(GPIO_PWM3, 0);
    pwmWrite(GPIO_PWM4, 0);

    //setup for I2C


    //setup for encoder pins


}

int main(){
    //testing if the lib set up correct
    int tsu = wiringPiSetup();
    int ti2 = wiringPiI2CSetup(IFSADR);
    if(tsu == -1 || ti2 == -1){
        return -1;
    }
    //pin setup here
    pinSetup();
    while(1){
        pwmWrite(GPIO_PWM1, 512);
        delay(500);
        pwmWrite(GPIO_PWM1, 0);
        delay(500);
        pwmWrite(GPIO_PWM3, 512);
        delay(500);
        pwmWrite(GPIO_PWM3, 0);
        delay(500);
    }

    return 0;
}
