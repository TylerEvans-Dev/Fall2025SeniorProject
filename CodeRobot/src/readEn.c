#include "../wiringOP/wiringPi/wiringPi.h"
#include <stdio.h>

//Code Tyler Evans and GPT5
//
void setup(int pinA, int pinB){
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
}


void memd(int* stepCount, int pinA, int pinB){
    //read encoder values

    int a = digitalRead(pinA);
    int b = digitalRead(pinB);
    //add or subtract based on that value.
    if(a == b){
        //if both are measured HIGH then add
        *stepCount++;
    }
    else{
        //if both are LOW sub.
        *stepCount--;
    }
}
