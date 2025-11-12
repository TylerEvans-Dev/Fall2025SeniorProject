#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#if defined(__APPLE__)
#include "../CodeTesting/GPIO/wiringOP/wiringPi/wiringPi.h"
#else
#include <wiringPi.h>
#include <softPwm.h>
#include <tof.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#endif
#include <fcntl.h>




//Define pins for PWM/IO
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

//Define duty cycle and clock speed for freq of PWM and direction
#define PWM_RANGE 1024
#define PWM_DIV 1
#define PWM_SLOW 250
#define PWM_MEDIUM 500
#define PWM_SOFT_CAP 1024
int left_offset = 200;//change this
int right_offset = 0;

//sets initial direction and defines different states for forward/backward/right/left/stop
#define DIR_STOP 0
int currentDir = DIR_STOP;
#define DIR_FORWARD 1
#define DIR_BACKWARD -1
#define DIR_RIGHT 2
#define DIR_LEFT 3

//Global encoder variables for counting encoder changes
volatile int32_t countlm = 0;
volatile uint8_t prevlm = 0;
volatile int32_t countrm = 0;
volatile uint8_t prevrm = 0;
const int8_t transTable[4][4] = {{0, 1, -1 , 0}, {-1, 0, 0, 1}, {1,0,0,-1}, {0,-1,1,0}}; //this is the trans. table for the encoder count.

//PID weights and error global variables
float kp = 20;
float ki = 0;
float kd = 0;
int32_t errorIntegral = 0;
int32_t prevError = 0;
int32_t lastCountL = 0;
int32_t lastCountR = 0;
//Global variables for I2C
#define I2C_BUS "/dev/i2c-4"
#define TCA_ADDR 0x70
#define TOF_ADDR 0x29
int multi = 0;
int fd;





//Stops all motors and has slight delay to prevent burnup
void stop(){
    pwmWrite(PWM1_PIN, 0);
    pwmWrite(PWM2_PIN, 0);
    pwmWrite(PWM3_PIN, 0);
    pwmWrite(PWM4_PIN, 0);
    digitalWrite(BRUSHL, LOW);
    softPwmWrite(BRUSHR, 0);
    digitalWrite(VACL, LOW);
    softPwmWrite(VACR, 0);
    currentDir = DIR_STOP;
    delay(10);
}

//TODO: need to verify working as expected and not nuisance
//sets the PWM and DO to a HIGH state in order to "lock" motors
void brake(){
    pwmWrite(PWM1_PIN, PWM_RANGE);
    pwmWrite(PWM2_PIN, PWM_RANGE);
    pwmWrite(PWM3_PIN, PWM_RANGE);
    pwmWrite(PWM4_PIN, PWM_RANGE);
    delay(100);

}

//TODO: needs PID tuning
//PWM forward command
void forward(int PWMval){
    if (currentDir == DIR_BACKWARD) {
        stop();
        errorIntegral = 0;
        prevError = 0;
    }
    currentDir = DIR_FORWARD;

    int error = countlm - countrm;
    errorIntegral += error;
    int derivative = error - prevError;
    prevError = error;
    float pid = kp * error + ki * errorIntegral + kd * derivative;
    int correction = (int)pid;
    int leftPWM = PWMval - correction - left_offset;
    int rightPWM = PWMval + correction - right_offset;
    if (leftPWM < PWM_SLOW) leftPWM = PWM_SLOW;
    if (leftPWM > PWM_SOFT_CAP) leftPWM = PWM_SOFT_CAP;
    if (rightPWM < PWM_SLOW) rightPWM = PWM_SLOW;
    if (rightPWM > PWM_SOFT_CAP) rightPWM = PWM_SOFT_CAP;
    pwmWrite(PWM1_PIN, rightPWM);
    pwmWrite(PWM2_PIN, 0);
    pwmWrite(PWM3_PIN, leftPWM);
    pwmWrite(PWM4_PIN, 0);
}

//TODO: needs PID tuning
//PWM backward command
void backward(int PWMval){
    if (currentDir == DIR_FORWARD){
        stop();
        errorIntegral = 0;
        prevError = 0;
    }
    currentDir = DIR_BACKWARD;

    int error = countlm - countrm;
    errorIntegral += error;
    int derivative = error - prevError;
    prevError = error;
    float pid = kp * error + ki * errorIntegral + kd * derivative;
    int correction = (int)pid;
    int leftPWM = PWMval - correction - right_offset;
    int rightPWM = PWMval + correction - left_offset;
    if (leftPWM < PWM_SLOW) leftPWM = PWM_SLOW;
    if (leftPWM > PWM_SOFT_CAP) leftPWM = PWM_SOFT_CAP;
    if (rightPWM < PWM_SLOW) rightPWM = PWM_SLOW;
    if (rightPWM > PWM_SOFT_CAP) rightPWM = PWM_SOFT_CAP;
    pwmWrite(PWM1_PIN, 0);
    pwmWrite(PWM2_PIN, rightPWM);
    pwmWrite(PWM3_PIN, 0);
    pwmWrite(PWM4_PIN, leftPWM);
}

//TODO: needs tuning after robot/motor changes
//Go specified distance in mm and given direction DIR_FOR/DIR_REV and start speed
void distance_cm(int cm, int direction, int speed) {
    int startr = countrm;
    int startl = countlm;
    int countrdist = 0;
    int countldist = 0;
    int countrcm = 0;
    int countlcm = 0;
    int ratio = 38;
    while (abs(countrcm) < cm && abs(countlcm) < cm) {
        countrdist = countrm - startr;
        countldist = countlm - startl;
        countrcm = countrdist / ratio;
        countlcm = countldist / ratio;
        if (direction == DIR_FORWARD) {
            if (speed > 0 && speed < PWM_RANGE) {
                forward(speed);
            }
        }
        else if (direction == DIR_BACKWARD) {
            if (speed > 0 && speed < PWM_RANGE) {
                backward(speed);
            }
        }
        else {
            printf("Invalid speed (negative speed)\n");
        }
    }
    brake();
    stop();
}

//Right chasis encoder counter
void encoder_r_isr(void){
    uint8_t a = digitalRead(P37);
    uint8_t b = digitalRead(P38);
    uint8_t cur = (a << 1) | b;
    countrm += transTable[prevrm][cur];
    prevrm = cur;
//    printf("The r value is %i \n", countrm);
}

//Left chasis encoder counter
void encoder_l_isr(void){
    uint8_t a = digitalRead(P11);
    uint8_t b = digitalRead(P12);
    uint8_t cur = (a << 1) | b;
    countlm += transTable[prevlm][cur];
    prevlm = cur;
//    printf("The l value is %i \n", countlm);
}

//Reads the encoder values of PCA I2C channels returns as array
uint16_t* read_PCA_channels(int ch) {
    static uint16_t distances[8];
    ioctl(fd, I2C_SLAVE, TCA_ADDR);
    uint8_t config = 1 << ch;
    write(fd, &config, 1);
    ioctl(fd, I2C_SLAVE, TOF_ADDR);
    if (multi < 8) {
        tofInit(4, TOF_ADDR, 1);
    }
    distances[ch] = tofReadDistance();
    return distances;
}

//TODO: this needs tuning badly
//Turn right, flip 180, and point to next parallel path in other direction
void turn(int direction) {
    int turncount = 1150; // number of encoder counts to flip 180 on one wheel
    int startr = countrm;
    int startl = countlm;
    int turnr = 0;
    int turnl = 0;
    // for (int i = 0; i < 2; i++) {
    //     while ((direction == DIR_RIGHT && abs(turnr)  < turncount) || (direction == DIR_LEFT && abs(turnl) < turncount)) {
    //         delay(500);
    //         if (direction == DIR_RIGHT) {
    //             pwmWrite(PWM3_PIN, PWM_MEDIUM);
    //             pwmWrite(PWM4_PIN, PWM_MEDIUM);
    //             turnr = countrm - startr;
    //         }
    //         if (direction == DIR_LEFT) {
    //             pwmWrite (PWM1_PIN, PWM_MEDIUM);
    //             pwmWrite (PWM2_PIN, PWM_MEDIUM);
    //             turnl = countlm - startl;
    //         }
    //     }
    //     brake();
    //     stop();
    //     if (i == 0) {
    //         delay(500);
    //         distance_cm(10, DIR_FORWARD, 120);
    //         turnr = 0;
    //         turnl = 0;
    //     }
    // }
    int i = 0;
    while(i > 2000000){
        pwmWrite(PWM1_PIN, PWM_MEDIUM);
        pwmWrite(PWM2_PIN, PWM_MEDIUM);
    }
    brake();
    stop();
}

//TODO: verify this isnt causing delay
//read sensors and go forward until it reads greater than
//200 take a verification measurment and stop
void look_for_edge(void){
    while(1) {
        uint16_t  *tofDistances =  read_PCA_channels(0);
        if (tofDistances[0] > 200) {
            delay(50);
            tofDistances = read_PCA_channels(0);
            if (tofDistances[0] > 200) {
                brake();
                stop();
                break;
            }
        }
        forward(PWM_MEDIUM);
    }
    delay(25);
    distance_cm(10, DIR_BACKWARD, PWM_MEDIUM);
    close(fd);
}

//TODO: verify that this is working with both vacuum and brush
void cleaning(){
    digitalWrite(BRUSHL, LOW); //soft pwm direction
    softPwmWrite(BRUSHR, 30); //soft pwm duty cycle %
    digitalWrite(VACL, LOW); //soft pwm direction
    softPwmWrite(VACR, 60); //soft pwm duty cycle %
}

//Sets the pins used for PWM to PWM_OUTPUT mode and establishes the
//max duty cydle & clock speed for PWM.
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

    //setup the clock speed and duty cycle range for chasis motors
    pwmSetRange(PWM1_PIN, PWM_RANGE);
    pwmSetClock(PWM1_PIN, PWM_DIV);
    pwmSetRange(PWM2_PIN, PWM_RANGE);
    pwmSetClock(PWM2_PIN, PWM_DIV);
    pwmSetRange(PWM3_PIN, PWM_RANGE);
    pwmSetClock(PWM3_PIN, PWM_DIV);
    pwmSetRange(PWM4_PIN, PWM_RANGE);
    pwmSetClock(PWM4_PIN, PWM_DIV);

    //setup DI pins for encoder reading chasis motors and start reading indefinetly
    pinMode(P12, INPUT);
    pinMode(P11, INPUT);
    pinMode(P37, INPUT);
    pinMode(P38, INPUT);
    prevlm = (digitalRead(P11) << 1) | digitalRead(P12);
    prevrm = (digitalRead(P37) << 1) | digitalRead(P38);
    wiringPiISR(P11, INT_EDGE_BOTH, &encoder_l_isr);
    wiringPiISR(P12, INT_EDGE_BOTH, &encoder_l_isr);
    wiringPiISR(P37, INT_EDGE_BOTH, &encoder_r_isr);
    wiringPiISR(P38, INT_EDGE_BOTH, &encoder_r_isr);

    //initialize multiplexer for tof sensors
    fd = open(I2C_BUS, O_RDWR);
    for (int ch=0; ch <8; ch++) {
        read_PCA_channels(ch);
        multi++;
    }
}





//main function
int main(void){
    //initializing pins and setup for robot
    wiringPiSetup();
    printf("WiringPi setup\n");
    setupRobot();
    printf("Robot setup\n");

    //start to do "main" loop and go back/fourth and clean
    cleaning();
    //look_for_edge();
    //
    // testing the turn should go left
    turn(DIR_LEFT);
    // turn should go right
    turn(DIR_RIGHT);
return 0;
}
