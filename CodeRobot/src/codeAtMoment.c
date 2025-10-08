#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include <sys/_select.h>
//#include <sys/_types/_fd_def.h>
#include <unistd.h>
#include <time.h>
//#include <sys/select.h>
#include <wiringPi.h>
//#include "read.c"
//#include "action.c"
//#include "cord.c"
//#include "beg.c"
#include <softPwm.h>
//#include "i2c.c"

//todo implement functions here.
void customMode(){

}
void mapping(){

}

void cleaning(){

}


#define PWM1_PIN 21
#define PWM2_PIN 22
#define PWM3_PIN 2
#define PWM4_PIN 9
#define P11 5
#define P12 6


#define MAX_RANGE_PWM 100
#define PWM_RANGE 1024
#define PWM_DIV 192
int count = 0; 
void setupPwm(){
    //setup the pinmode
    pinMode(PWM1_PIN, PWM_OUTPUT);
    pinMode(PWM2_PIN, PWM_OUTPUT);
    pinMode(PWM3_PIN, PWM_OUTPUT);
    pinMode(PWM4_PIN, PWM_OUTPUT);
    //setup the PWM mode PWM_MODE_MS
   // pwmSetMode(PWM1_PIN, PWM_MODE_MS); // PWM 1
   // pwmSetMode(PWM2_PIN, PWM_MODE_MS); // PWM 2
    //pwmSetMode(PWM3_PIN, PWM_MODE_MS); // PWM 3
   // pwmSetMode(PWM4_PIN, PWM_MODE_MS); // PWM 4
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
void encoderRead( int pinA, int pinB){
 //read encoder values
    int a = digitalRead(pinA);
    int b = digitalRead(pinB);
    printf("value of a is %i and value of b is %i \n", a, b);
        if(a == 1){
        printf("forwards\n");
        count += 1;
        }
        else{
        printf("backwards\n");
        count -=1;
        }

}
volatile uint32_t countlm = 0;
volatile uint8_t prvlm;
volatile uint32_t  countrm = 0;
volatile uint8_t prvrm;
//this is the trans. table for the encoder count.
const uint8_t transTable[4][4] = {{0, 1, -1 , 0}, {-1, 0, 0, 1}, {1,0,0,-1}, {0, -1,1,0}};

void encoder_r_isr(void){
 uint8_t a = digitalRead(P11);
 uint8_t b = digitalRead(P12);
 //shifting the read so it becomes the higher bit.
 uint8_t cur = (a<<1) | b;
 countrm += transTable[prvrm][cur];
 prvrm = cur;
printf("the value is %i \n", countlm);
}

void encoder_l_isr(void){
    uint8_t a = digitalRead(P11);
    uint8_t b = digitalRead(P12);
    //shifting the read so it becomes the higher bit.
    uint8_t cur = (a<<1) | b;
    countlm += transTable[prvlm][cur];
    prvlm = cur;
    printf("the value is %i \n", countrm);
    }



//main function
int main(){
        wiringPiSetup();
        pinMode(P12, INPUT);
        pinMode(P11, INPUT);
         if(wiringPiISR(P11, INT_EDGE_BOTH, &encoder_r_isr) < 0){
                printf("failure to use right ISR\n");
                return -1;
        }
        if(wiringPiISR(P12, INT_EDGE_BOTH, &encoder_l_isr) < 0){
                printf("failure to use left ISR\n");
                return -1;
        }
    //if(beginLoop() == 1){
        customMode();
        setupPwm();
        printf("working in custom mode");
        while(1){
                forward(200);
                //encoderRead(P11, P12);
                delay(1000);
                stop();
                delay(1000);
                backward(200);

                delay(1000);
                //memd(j, P11, P12);
                //encoderRead(P11, P12);
                stop();
                printf("the count is %i", count);
                delay(1000);
        }
    //}
   // else{
        printf("working in default mode");
       // printf("the value read %i", lol());
    //}
    return 0;
}
