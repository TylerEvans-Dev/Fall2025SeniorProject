//used for the main program
#include <stdio.h>
#include "ir.h"
#include "timing.c"
#include <unistd.h>
//used system dependent
#if defined(__APPLE__)
#include "../GPIO/wiringOP/wiringPi/wiringPi.h"
#else
#include <wiringPi.h>
#endif
#define PIN39 39
#define PIN40 40
int main(){
    int c = wiringPiSetup();
    if(!c){
        printf("there was an issue with setup :( \n");
    }
    //used to init the pins
    initPinIR(PIN39);
    initPinIR(PIN40);
    //while loop for testing
    while(1){
        //to test this sensor I used the timing.c to measure the time
        check_run_time(readPinIR, PIN39, "testing read if there is pin.");
        check_run_time(readIRObj, PIN40,    "testing time to read if an object present.");
        printf("the value for measured for pin 39 %i \n", readPinIR(PIN39));
        printf("the value for the measured for pin 40 %i \n", readIRObj(PIN40));
        usleep(1000);
    }
}
