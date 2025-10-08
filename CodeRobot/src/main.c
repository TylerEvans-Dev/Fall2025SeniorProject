#include <stdint.h>
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
#define P37 26
#define P38 25

//todo implement functions here.

int encoderRead(int pinA){
    int a = digitalRead(pinA);
    if(a == 1){
        return 1;
    }
    else{
        return 0;
    }
}


//TODO define these values exact
//
// TODO define these vaues here for the encoder count.
#define PIN_EN1 20
#define PIN_EN2 19
#define PIN_EN3 18
#define PIN_EN4 17

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
    pwmSetRange(PWM_RANGE); //resolution PWM 1

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

//needed for encoder counts.
volatile uint32_t countlm = 0;
volatile uint8_t prvlm;
volatile uint32_t  countrm = 0;
volatile uint8_t prvrm;
//this is the trans. table for the encoder count.
const uint8_t transTable[4][4] = {{0, 1, -1 , 0}, {-1, 0, 0, 1}, {1,0,0,-1}, {0, -1,1,0}};

void encoder_r_isr(void){
 uint8_t a = digitalRead(PIN_EN1);
 uint8_t b = digitalRead(PIN_EN2);
 //shifting the read so it becomes the higher bit.
 uint8_t cur = (a<<1) | b;
 countrm += transTable[prvrm][cur];
 prvrm = cur;
}

void encoder_l_isr(void){
    uint8_t a = digitalRead(PIN_EN3);
    uint8_t b = digitalRead(PIN_EN4);
    //shifting the read so it becomes the higher bit.
    uint8_t cur = (a<<1) | b;
    countrm += transTable[prvlm][cur];
    prvlm = cur;
}

//main function
int main(){
    if(wiringPiISR(PIN_EN1, INT_EDGE_BOTH, &encoder_r_isr) < 0){
        printf("failure to use right ISR\n");
        return -1;
    }
    if(wiringPiISR(PIN_EN3, INT_EDGE_BOTH, &encoder_l_isr) < 0){
        printf("failure to use left ISR\n");
    }


    if(beginLoop() == 1){
        //customMode();
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
