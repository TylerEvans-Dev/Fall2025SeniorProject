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
//every action made must be put at rest or it will damage the motor.
void forward(int PWMval){
    stop(); // must be done to help motors be good
    softPwmWrite(PWM3_PIN, PWMval);
    softPwmWrite(PWM1_PIN, PWMval);
}

void backward(int PWMval){
    stop(); //must be done to help motor health
    softPwmWrite(PWM4_PIN, PWMval);
    softPwmWrite(PWM2_PIN, PWMval);
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
