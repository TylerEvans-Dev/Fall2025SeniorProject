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
int initGY521(const char *chan, int *fd, int *check);

//these are the features of the GY521 breakout board
uint8_t readWHOAMI(int *fd);
int mpu_read_all(int *fd, DataAccel *data);
float readXAce(int *fd, int check);
float readYAce(int *fd, int check);
float readZAce(int *fd, int check);
float readXGyr(int *fd, int check);
float readYGyr(int *fd, int check);
float readZGyr(int *fd, int check);
float readTemp(int *fd, int check);






#endif
