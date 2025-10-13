#define GY521_H
#ifdef GY521_H
#if defined(__APPLE__)
#include "../GPIO/wiringOP/wiringPi/wiringPi.h"
#include "../GPIO/wiringOP/wiringPi/wiringPiI2C.h"
#else
#include <wiringPi.h>
#include <wiringPiI2C.h>
#endif
#include <stdint.h>
#include <stdio.h>
//this is the file desciptor
static int setup = 0;
static int scaleAcel;
static int scaleGyro;
typedef struct{
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
    float temp;
}DataAccel;
//this will initallize the GY521
int initGY521(const char *chan, int *fd);

//these are the features of the GY521 breakout board
uint8_t readWHOAMI(int *fd);
int mpu6050_read_all(int *fd, DataAccel *data);
uint8_t readXAce();
uint8_t readYAce();
uint8_t readZAce();
uint8_t readXGyr();
uint8_t readYGyr();
uint8_t readZGyr();
uint8_t readTemp();






#endif
