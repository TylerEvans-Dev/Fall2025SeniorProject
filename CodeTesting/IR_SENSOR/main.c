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
#define PIN39 26
#define PIN40 27
int main(){
    int c = wiringPiSetup();
    if(c == -1){
        printf("there was an issue with setup :( \n");
    }
    //used to init the pins
    initPinIR(PIN39);
    initPinIR(PIN40);
    //while loop for testing
    while(1){
        //to test this sensor I used the timing.c to measure the time
        printf("\n");
        for(int i=0; i < 10; i++){
        check_run_time(readPinIR, PIN39, "testing read if there is pin.\n");
        check_run_time(readIRObj, PIN40,    "testing time to read if an object present.\n");
        }
        printf("\n");
        for(int j = 0; j <20; j++){
        printf("the value for measured for pin 39 %i \n", readPinIR(PIN39));
        printf("the value for the measured for pin 40 %i \n", readIRObj(PIN40));
        }
        sleep(1);
    }
}
