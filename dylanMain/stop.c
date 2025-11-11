

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wiringPi.h>
#include <softPwm.h>

#define PWM1_PIN 21 //right motor fwd
#define PWM2_PIN 22 //right motor rev
#define PWM3_PIN 2 //left motor fwd
#define PWM4_PIN 9 //left motor rev
#define P11 5 //encoder left
#define P12 6 //encoder left
#define P37 25 //encoder right
#define P38 26 //encoder right
#define BRUSHL 27 //brush left PWM
#define BRUSHR 23 //brush right PWM
#define VACL 24 //vacuum left PWM
#define VACR 20 //vacuum right PWM

void stop() {
    pwmWrite(PWM1_PIN, 0);
    pwmWrite(PWM2_PIN, 0);
    pwmWrite(PWM3_PIN, 0);
    pwmWrite(PWM4_PIN, 0);
    digitalWrite(BRUSHL, LOW);
    softPwmWrite(BRUSHR, 0);
    digitalWrite(VACL, LOW);
    softPwmWrite(VACR, 0);
    delay(10);
}

void setupRobot(){
    //setup the pinmode and initialize to zero for chasis motors
    pinMode(PWM1_PIN, PWM_OUTPUT);
    pwmWrite(PWM1_PIN, 0);
    pinMode(PWM2_PIN, PWM_OUTPUT);
    pwmWrite(PWM2_PIN, 0);
    pinMode(PWM3_PIN, PWM_OUTPUT);
    pwmWrite(PWM3_PIN, 0);
    pinMode(PWM4_PIN, PWM_OUTPUT);
    pwmWrite(PWM4_PIN, 0);

    //setup soft pwm for vacuum/brush
    pinMode(BRUSHL, OUTPUT);
    digitalWrite(BRUSHL, LOW);
    softPwmCreate(BRUSHR, 0, 100);
    softPwmWrite(BRUSHR, 0);
    pinMode(VACL, OUTPUT);
    digitalWrite(VACL, LOW);
    softPwmCreate(VACR, 0, 100);
    softPwmWrite(VACR, 0);
}

//main function
int main(){
    wiringPiSetup();
    setupRobot();
    stop();
    return 0;
}
