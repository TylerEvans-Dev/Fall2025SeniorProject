#include <stdio.h>
#include <stdlib.h>
#include <sys/_select.h>
#include <sys/_types/_fd_def.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>
#include "../wiringOP/wiringPi/wiringPi.h"
//#include "read.c"
//#include "action.c"
//#include "cord.c"
#include "beg.c"
#include "../wiringOP/wiringPi/softPwm.h"
//#include "i2c.c"

//todo implement functions here.
void customMode(){

}
void mapping(){

}

void cleaning(){

}

void memd(int* stepCount, int pinA, int pinB){
    //read encoder values
    int a = digitalRead(pinA);
    int b = digitalRead(pinB);
    //add or subtract based on that value.
    if(a == b){
        //if both are measured HIGH then add
        *stepCount++;
        printf("step");
    }
    else{
        //if both are LOW sub.
        *stepCount--;
        printf("neg step");
    }
}

//TODO define these values exact
#define PWM1_PIN 21
#define PWM2_PIN 22
#define PWM3_PIN 2
#define PWM4_PIN 9

#define MAX_RANGE_PWM 100
#define PWM_RANGE 1024
#define PWM_DIV 192

void setupPwm(){
    //setup the pinmode
    pinMode(PWM1_PIN, PWM_OUTPUT);
    pinMode(PWM2_PIN, PWM_OUTPUT);
    pinMode(PWM3_PIN, PWM_OUTPUT);
    pinMode(PWM4_PIN, PWM_OUTPUT);
    //setup the PWM mode PWM_MODE_MS
    pwmSetMode(PWM1_PIN, PWM_MODE_MS); // PWM 1
    pwmSetMode(PWM2_PIN, PWM_MODE_MS); // PWM 2
    pwmSetMode(PWM3_PIN, PWM_MODE_MS); // PWM 3
    pwmSetMode(PWM4_PIN, PWM_MODE_MS); // PWM 4
    //set clock speed.
    pwmSetRange(PWM1_PIN, PWM_RANGE); //resolution PWM 1
    pwmSetRange(PWM2_PIN, PWM_RANGE); //resolution PWM 2
    pwmSetRange(PWM3_PIN, PWM_RANGE); //resolution PWM 3
    pwmSetRange(PWM4_PIN, PWM_RANGE); //resolution PWM 4

    pwmSetClock(PWM1_PIN, PWM_DIV); //div PWM 1
    pwmSetClock(PWM2_PIN, PWM_DIV); //div PWM 2
    pwmSetClock(PWM3_PIN, PWM_DIV); //div PWM 3
    pwmSetClock(PWM4_PIN, PWM_DIV); //div PWM 4
}

void stop(){
    //must stop all to so set to zero.
    pwmWrite(PWM1_PIN, 0);
    pwmWrite(PWM2_PIN, 0);
    pwmWrite(PWM3_PIN, 0);
    pwmWrite(PWM4_PIN, 0);
    usleep(10); //added delay to help motor slow down
}
//every action made must be put at rest or it will damage the motor.
void forward(int PWMval){
    stop(); // must be done to help motors be good
    pwmWrite(PWM3_PIN, PWMval);
    pwmWrite(PWM1_PIN, PWMval);

}

void backward(int PWMval){
    stop(); //must be done to help motor health
    pwmWrite(PWM4_PIN, PWMval);
    pwmWrite(PWM2_PIN, PWMval);
}

void turn(float deg){
    //TODO
}

//main function
int main(){
    if(beginLoop() == 1){
        customMode();
        setupPwm();
        printf("working in custom mode");
        while(1){
        forward(50);
        delay(100);
        backward(50);
        delay(100);
        }
    }
    else{
        printf("working in default mode");
       // printf("the value read %i", lol());
    }
    return 0;
}
