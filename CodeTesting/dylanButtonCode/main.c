#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#if defined(__APPLE__)
#include "../GPIO/wiringOP/wiringPi/wiringPi.h"
#include "../GPIO/wiringOP/wiringPi/wiringPiI2C.h"
#else
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <softPwm.h>
#include <tof.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#endif
#include <stdbool.h>



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

//added button section to be clear with
// The reason for the high is for logic high voltage
// to give the logic voltage so when the button is pushed
// you can use this
#define BUTTON_START_READ  19 //pin 29 physical
#define BUTTON_START_HIGH  7 //pin 13  physical

#define BUTTON_STOP_READ 10 //pin 18 physical
#define BUTTON_STOP_HIGH 8 //pin 15 physical

int shouldloop = 0; //off by default.

//Define duty cycle and clock speed for freq of PWM and direction
#define PWM_RANGE 1024
#define PWM_DIV 1
#define PWM_SLOW 250
#define PWM_MEDIUM 300
#define PWM_SOFT_CAP 1024 - 300
int left_offset = 200;//change this
int right_offset = 00;


//sets initial direction and defines different states for forward/backward/right/left/stop
#define DIR_STOP 0
int currentDir = DIR_STOP;
#define DIR_FORWARD 1
#define DIR_BACKWARD -1
#define DIR_RIGHT 2
#define DIR_LEFT 3
int edge_distance = 150;
bool on_edge = false;
bool turning = false;
bool startup = false;

//Global encoder variables for counting encoder changes
volatile int32_t countlm = 0;
volatile uint8_t prevlm = 0;
volatile int32_t countrm = 0;
volatile uint8_t prevrm = 0;
const int8_t transTable[4][4] = {{0, 1, -1 , 0}, {-1, 0, 0, 1}, {1,0,0,-1}, {0,-1,1,0}}; //this is the trans. table for the encoder count.

//PID weights and error global variables
float kp = 1.5;
float ki = .008;
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



//Stops all chasis motors and has slight delay to prevent burnup
void stop(){
    pwmWrite(PWM1_PIN, 0);
    pwmWrite(PWM2_PIN, 0);
    pwmWrite(PWM3_PIN, 0);
    pwmWrite(PWM4_PIN, 0);
    currentDir = DIR_STOP;
    delay(500);
}

void readStop(void){
    shouldloop = 0; //turns loop off
    printf("reading stop button shouldloop == %i\n", shouldloop);
}

void readStart(void){
    shouldloop =1; //turns loop on.
    printf("reading start button shouldloop == %i\n", shouldloop); //for debug statement
}

void setupButton(){
    //for digital read and high
    pinMode(BUTTON_START_READ, INPUT);
    pinMode(BUTTON_START_HIGH, OUTPUT);
    //setting this high for read
    digitalWrite(BUTTON_START_HIGH, HIGH);

    //for digital read and set.
    pinMode(BUTTON_STOP_READ, INPUT);
    pinMode(BUTTON_STOP_HIGH, OUTPUT);
    //setting this high for read
    digitalWrite(BUTTON_STOP_HIGH, HIGH);
    wiringPiISR(BUTTON_STOP_READ,INT_EDGE_RISING, &readStop);
    wiringPiISR(BUTTON_START_READ, INT_EDGE_RISING, &readStart);
}

//Stops the brush and vacuum
void stop_cleaning() {
    digitalWrite(BRUSHL, LOW);
    softPwmWrite(BRUSHR, 0);
    digitalWrite(VACL, LOW);
    softPwmWrite(VACR, 0);
    delay(2000);
    close(fd);
}


//sets the PWM and DO to a HIGH state in order to "lock" motors
void brake(){
    pwmWrite(PWM1_PIN, PWM_RANGE);
    pwmWrite(PWM2_PIN, PWM_RANGE);
    pwmWrite(PWM3_PIN, PWM_RANGE);
    pwmWrite(PWM4_PIN, PWM_RANGE);
    delay(100);

}

//resets encoder counts
void reset_count() {
    countrm = 0;
    countlm = 0;
    prevrm = 0;
    prevlm = 0;
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
    int leftPWM = PWMval + correction - left_offset;
    int rightPWM = PWMval - correction - right_offset;
    if (leftPWM < PWM_SLOW) leftPWM = PWM_SLOW;
    if (leftPWM > PWM_SOFT_CAP) leftPWM = PWM_SOFT_CAP;
    if (rightPWM < PWM_SLOW) rightPWM = PWM_SLOW;
    if (rightPWM > PWM_SOFT_CAP) rightPWM = PWM_SOFT_CAP;
    pwmWrite(PWM1_PIN, 0);
    pwmWrite(PWM2_PIN, rightPWM);
    pwmWrite(PWM3_PIN, 0);
    pwmWrite(PWM4_PIN, leftPWM);
}

//Reads the encoder values of PCA I2C channels returns as int
int read_PCA_channels(int ch) {
    ioctl(fd, I2C_SLAVE, TCA_ADDR);
    uint8_t config = 1 << ch;
    write(fd, &config, 1);
    ioctl(fd, I2C_SLAVE, TOF_ADDR);
    if (multi < 8) {
        tofInit(4, TOF_ADDR, 1);
    }
    int distance = tofReadDistance();
//    printf("The tof distance is %d\n", distance);
    return distance;
}

//Go specified distance in mm and given direction DIR_FOR/DIR_REV and start speed
void distance_cm(int cm, int direction, int speed) {
    int startr = countrm;
    int startl = countlm;
    int countrdist = 0;
    int countldist = 0;
    int countrcm = 0;
    int countlcm = 0;
    int ratio = 60;
    int leftsense = read_PCA_channels(0);
    int rightsense = read_PCA_channels(1);
    while (abs(countrcm) < cm && abs(countlcm) < cm) {
        leftsense = read_PCA_channels(0);
        rightsense = read_PCA_channels(1);
        if((leftsense > edge_distance || rightsense > edge_distance) && turning) {
            brake();
            stop();
            on_edge = true;
            return;
        }
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
            printf("Invalid direction\n");
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

//Squares the robot with an edge to prep for next pass
void square(int direction) {
    int left = 0;
    int right = 0;
    int redge = 0;
    int ledge = 0;
    while(redge != 1 || ledge != 1) {
        if(direction == DIR_FORWARD) {
            if (left < edge_distance) {
                pwmWrite(PWM1_PIN, PWM_SLOW + 70);
                pwmWrite(PWM2_PIN, 0);
                left = read_PCA_channels(0);
            }
            else {
                ledge = 1;
                pwmWrite(PWM1_PIN, PWM_RANGE);
                pwmWrite(PWM2_PIN, PWM_RANGE);
                delay(50);
                pwmWrite(PWM1_PIN, 0);
                pwmWrite(PWM2_PIN, 0);
            }

            if (right < edge_distance) {
                pwmWrite(PWM3_PIN, PWM_SLOW);
                pwmWrite(PWM4_PIN, 0);
                right = read_PCA_channels(1);
            }
            else {
                redge = 1;
                pwmWrite(PWM3_PIN, PWM_RANGE);
                pwmWrite(PWM4_PIN, PWM_RANGE);
                delay(50);
                pwmWrite(PWM3_PIN, 0);
                pwmWrite(PWM4_PIN, 0);
            }
        }


        if (direction == DIR_BACKWARD) {
            if (left < edge_distance) {
                pwmWrite(PWM1_PIN, 0);
                pwmWrite(PWM2_PIN, PWM_SLOW + 70);
                left = read_PCA_channels(2);
            }
            else {
                ledge = 1;
                pwmWrite(PWM1_PIN, PWM_RANGE);
                pwmWrite(PWM2_PIN, PWM_RANGE);
            }

            if (right < edge_distance) {
                pwmWrite(PWM3_PIN, 0);
                pwmWrite(PWM4_PIN, PWM_SLOW);
                right = read_PCA_channels(3);
            }
            else {
                redge = 1;
                pwmWrite(PWM3_PIN, PWM_RANGE);
                pwmWrite(PWM4_PIN, PWM_RANGE);
            }
        }
    }
    delay(200);
    stop();
    reset_count();
}

//Turn 90 deg right or left
void turn(int direction) {
    int turncount = 1600; // number of encoder counts to flip 180 on one wheel
    int startr = countrm;
    int startl = countlm;
    int turnr = 0;
    int turnl = 0;
    while ((direction == DIR_RIGHT && abs(turnr)  < turncount) || (direction == DIR_LEFT && abs(turnl) < turncount)) {
        if (direction == DIR_RIGHT) {
            pwmWrite(PWM3_PIN, 0);
            pwmWrite(PWM4_PIN, PWM_MEDIUM + 100);
            turnr = countrm - startr;
        }
        if (direction == DIR_LEFT) {
            pwmWrite (PWM1_PIN, 0);
            pwmWrite (PWM2_PIN, PWM_MEDIUM + 100);
            turnl = countlm - startl;
        }
    }
    brake();
    stop();
    reset_count();
}

//read sensors and go forward until it reads greater than
//edge_distance(distance from sensor to ground) compare to other side and stop and stop
//if one side goes over edge adjust motors to correct
void look_for_edge(void){
    while(1) {
        int tofl = 0;
        int tofr = 0;
        tofl = read_PCA_channels(0);
        tofr = read_PCA_channels(1);
        if(tofr > edge_distance && !(tofl > edge_distance)) {
            delay(10);
            tofr = read_PCA_channels(1);
            if(tofr > edge_distance) {
                pwmWrite(PWM1_PIN, 0);
                pwmWrite(PWM2_PIN, PWM_MEDIUM);
                reset_count();
                delay(10);
            }
        }
        if(tofl > edge_distance && !(tofr > edge_distance)) {
            delay(10);
            tofl = read_PCA_channels(0);
            if(tofl > edge_distance) {
                pwmWrite(PWM3_PIN, 0);
                pwmWrite(PWM4_PIN, PWM_MEDIUM);
                reset_count();
                delay(10);
            }
        }
        if (tofr > edge_distance) {
            if (tofl > edge_distance) {
                brake();
                stop();
                break;
            }
        }
        forward(PWM_SLOW);
    }
    distance_cm(10, DIR_BACKWARD, PWM_SLOW);
    square(DIR_FORWARD);
    if(!startup) {
        distance_cm(10, DIR_BACKWARD, PWM_SLOW);
    }
    else {
        distance_cm(2, DIR_FORWARD, PWM_SLOW);
    }
}

//Turn in direction and move over to next row
void next_row(int direction) {
    turn(direction);
    turning = true;
    distance_cm(30, DIR_FORWARD, PWM_SLOW);
    turn(direction);
    square(DIR_BACKWARD);
    turning = false;
    distance_cm(10, DIR_FORWARD, PWM_SLOW);
    square(DIR_BACKWARD);
}

//this starts the vacuum and brush for cleaning
void cleaning(){
    digitalWrite(BRUSHL, LOW); //soft pwm direction
    softPwmWrite(BRUSHR, 40); //soft pwm duty cycle %
    digitalWrite(VACL, LOW); //soft pwm direction
    softPwmWrite(VACR, 100); //soft pwm duty cycle %
}

//robot finds an edge and squares with edge to determine starting position.
void start_position() {
    startup = true;
    look_for_edge();
    turn(DIR_RIGHT);
    look_for_edge();
    turn(DIR_RIGHT);
    square(DIR_BACKWARD);
    startup = false;
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

    //start to do "main" loop and go back/fourth and clean
    while(shouldloop == 1){
    cleaning();
    start_position();
    while(!on_edge) {
        look_for_edge();
        next_row(DIR_RIGHT);
        look_for_edge();
        next_row(DIR_LEFT);
    }
    startup = true;
    look_for_edge();
    startup = false;
    turn(DIR_RIGHT);
    on_edge = false;
    delay(100);
    while(!on_edge) {
        look_for_edge();
        next_row(DIR_RIGHT);
        look_for_edge();
        next_row(DIR_LEFT);
    }
    look_for_edge();
    loopend:
    stop_cleaning();
    }
    while(shouldloop == 0){
        printf("idle\n");
        usleep(100);
    }
/*    for(int i = 0; i<4; i++ ) {
        read_PCA_channels(i);
    } */
}
