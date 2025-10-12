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
static int fd;
static int scaleAcel;
static int scaleGyro;
//this will initallize the GY521
int initGY521(const char *chan);

//these are the features of the GY521 breakout board
uint8_t readWHOAMI();
uint8_t readXAce();
uint8_t readYAce();
uint8_t readZAce();
uint8_t readXGyr();
uint8_t readYGyr();
uint8_t readZGyr();
uint8_t readTemp();






#endif
