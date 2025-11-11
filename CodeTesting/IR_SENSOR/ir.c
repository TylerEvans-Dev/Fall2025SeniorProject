#include "ir.h"
#include <stdio.h>
#if defined(__APPLE__)
#include "../GPIO/wiringOP/wiringPi/wiringPi.h"
#else
#include <wiringPi.h>
#endif

void initPinIR(int PIN){
    pinMode(PIN, INPUT);
}

int readPinIR(int PIN){
    int val = digitalRead(PIN);
    printf("GGGGGG the value red is %i \n ", val);
    //returns 1 if on surface pin will be high
    if(val){
        return 1;
    }
    //returns 0 if on edge pin will be held low
    else{
        return 0;
    }
}

int readIRObj(int PIN){
    int val = digitalRead(PIN);
    //returns 1 if object present pin will be high
    if(val){
        return 1;
    }
    //returns 0 if object not present pin will be LOW
    else{
        return 0;
    }
}
