#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <tof.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
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

//Define duty cycle and clock speed for freq of PWM and direction
#define PWM_RANGE 1024
#define PWM_DIV 1
#define DIR_STOP 0
#define DIR_FORWARD 1
#define DIR_BACKWARD -1
#define DIR_RIGHT 2
#define DIR_LEFT 3
int currentDir = DIR_STOP;

//Global encoder variables for counting encoder changes
volatile int32_t countlm = 0;
volatile uint8_t prevlm = 0;
volatile int32_t countrm = 0;
volatile uint8_t prevrm = 0;
volatile int32_t error = 0;
const int8_t transTable[4][4] = {{0, 1, -1 , 0}, {-1, 0, 0, 1}, {1,0,0,-1}, {0,-1,1,0}}; //this is the trans. table for the encoder count.

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
    currentDir = DIR_STOP;
    delay(10);
}

//PWM forward command
void forward(int PWMval){
    if (currentDir == DIR_BACKWARD) {
        stop();
    }
    currentDir = DIR_FORWARD;

    error = countrm - countlm;
    float Kp = .9;//change to tune the forward direction
    int correction = (int)(Kp * error);
    int leftPWM = PWMval - correction - 40;
    int rightPWM = PWMval + correction;
    if (leftPWM < 120) leftPWM = 120;
    if (leftPWM > PWM_RANGE - 800) leftPWM = PWM_RANGE - 800;
    if (rightPWM < 120) rightPWM = 120;
    if (rightPWM > PWM_RANGE - 800) rightPWM = PWM_RANGE - 800;
    pwmWrite(PWM1_PIN, rightPWM);
    pwmWrite(PWM2_PIN, 0);
    pwmWrite(PWM3_PIN, leftPWM);
    pwmWrite(PWM4_PIN, 0);
}

//PWM backward command
void backward(int PWMval){
    if (currentDir == DIR_FORWARD){
        stop();
    }
    currentDir = DIR_BACKWARD;

    error = countrm - countlm;
    float Kp = .9;//change to tune the backward direction
    int correction = (int)(Kp * error);
    int leftPWM = PWMval + correction;
    int rightPWM = PWMval - correction - 40;
    if (leftPWM < 120) leftPWM = 120;
    if (leftPWM > PWM_RANGE - 800) leftPWM = PWM_RANGE - 800;
    if (rightPWM < 120) rightPWM = 120;
    if (rightPWM > PWM_RANGE - 800) rightPWM = PWM_RANGE - 800;
    pwmWrite(PWM1_PIN, 0);
    pwmWrite(PWM2_PIN, rightPWM);
    pwmWrite(PWM3_PIN, 0);
    pwmWrite(PWM4_PIN, leftPWM);
}

//Go specified distance in mm and given direction DIR_FOR/DIR_REV and start speed TODO
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
    stop();
    if (direction == DIR_BACKWARD) backward(100);
    if (direction == DIR_FORWARD) forward(100);
    delay(300);
    stop();
}

//Right encoder counter
void encoder_r_isr(void){
    uint8_t a = digitalRead(P37);
    uint8_t b = digitalRead(P38);
    uint8_t cur = (a << 1) | b;
    countrm += transTable[prevrm][cur];
    prevrm = cur;
//    printf("The r value is %i \n", countrm);
}

//Left encoder counter
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

//Turn right and flip to other direction TODO
void turn(int direction) {
    int turncount = 1150; // number of encoder counts to flip 180 on one wheel
    int startr = countrm;
    int startl = countlm;
    int turnr = 0;
    int turnl = 0;
    for (int i = 0; i < 2; i++) {
        while ((direction == DIR_RIGHT && abs(turnr)  < turncount) || (direction == DIR_LEFT && abs(turnl) < turncount)) {
            delay(500);
            if (direction == DIR_RIGHT) {
                pwmWrite(PWM3_PIN, 0);
                pwmWrite(PWM4_PIN, 150);
                turnr = countrm - startr;
            }
            if (direction == DIR_LEFT) {
                pwmWrite (PWM1_PIN, 0);
                pwmWrite (PWM2_PIN, 120);
                turnl = countlm - startl;
            }
        }
        stop();
        if (i == 0) {
            delay(500);
            distance_cm(10, DIR_FORWARD, 120);
            turnr = 0;
            turnl = 0;
        }
/*    while() {
        uint16_t  *tofDistances =  read_PCA_channels(0);
        if (tofDistances[0] > 200) {
            delay(10);
            if (tofDistances[0] > 200) {
                stop();
                break;
            }
        }
        backward(100);
    } */
    }
    stop();
}

//TODO:
void look_for_edge(void){
    while(1) {
        uint16_t  *tofDistances =  read_PCA_channels(0);
        if (tofDistances[0] > 200) {
            delay(10);
            if (tofDistances[0] > 200) {
                stop();
                break;
            }
        }
        forward(150);
    }
    delay(1000);
    distance_cm(10, DIR_BACKWARD, 100);
    close(fd);
}

//TODO:
void cleaning(){

}

//Sets the pins used for PWM to PWM_OUTPUT mode and establishes the
//max duty cydle & clock speed for PWM.
void setupRobot(){
    //setup the pinmode and initialize to zero digital out used to prevent PWM sync
    pinMode(PWM1_PIN, PWM_OUTPUT);
    pwmWrite(PWM1_PIN, 0);
    pinMode(PWM2_PIN, PWM_OUTPUT);
    pwmWrite(PWM2_PIN, 0);
    pinMode(PWM3_PIN, PWM_OUTPUT);
    pwmWrite(PWM3_PIN, 0);
    pinMode(PWM4_PIN, PWM_OUTPUT);
    pwmWrite(PWM4_PIN, 0);
    //setup the clock speed and duty cycle range
    pwmSetRange(PWM1_PIN, PWM_RANGE);
    pwmSetClock(PWM1_PIN, PWM_DIV);
    pwmSetRange(PWM2_PIN, PWM_RANGE);
    pwmSetClock(PWM2_PIN, PWM_DIV);
    pwmSetRange(PWM3_PIN, PWM_RANGE);
    pwmSetClock(PWM3_PIN, PWM_DIV);
    pwmSetRange(PWM4_PIN, PWM_RANGE);
    pwmSetClock(PWM4_PIN, PWM_DIV);
    //setup AI pins for encoder reading
    pinMode(P12, INPUT);
    pinMode(P11, INPUT);
    pinMode(P37, INPUT);
    pinMode(P38, INPUT);
    //setup encoders to start reading
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
int main(){
    //initializing pins and setup for robot
    wiringPiSetup();
    printf("WiringPi setup\n");
    setupRobot();
    printf("Robot setup\n");
    look_for_edge();
    turn(DIR_RIGHT);
    look_for_edge();
    turn(DIR_LEFT);
return 0;
}
