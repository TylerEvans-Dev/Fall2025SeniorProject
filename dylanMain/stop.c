

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wiringPi.h>
#include <softPwm.h>
#define PWM4_PIN 9
#define MAX_RANGE_PWM 100
#define PWM1_PIN 21
#define PWM2_PIN 22
#define PWM3_PIN 2
#define PWM4_PIN 9
#define MAX_RANGE_PWM 100

void setupPwm(){
    //belong to motor 1
    softPwmCreate(PWM1_PIN, 0, MAX_RANGE_PWM); //red pin
    softPwmCreate(PWM2_PIN, 0, MAX_RANGE_PWM); //black pin
    //belong to motor 2
    softPwmCreate(PWM3_PIN, 0, MAX_RANGE_PWM);//red pin
    softPwmCreate(PWM4_PIN, 0, MAX_RANGE_PWM); //black pin
}

void stop(){
    //must stop all to so set to zero.
    softPwmWrite(PWM1_PIN, 0);
    softPwmWrite(PWM2_PIN, 0);
    softPwmWrite(PWM3_PIN, 0);
    softPwmWrite(PWM4_PIN, 0);
    usleep(10); //added delay to help motor slow down
}

void turn(float deg){
    //TODO
}

//main function
int main(){
        wiringPiSetup();
        setupPwm();
        stop();
    return 0;
}
