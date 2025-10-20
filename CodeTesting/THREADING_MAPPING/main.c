#include <pthread.h>
#include <stdio.h>
//accel. for reading a sensor for one thread
//#include "GY521_MPU/GY521.h"
#include <unistd.h>
#include "timing.c"

void testFunction1(){
    sleep(5);
    printf("function 1 \n");
}

void testFunction2(){
    usleep(10000);
    printf("function 2 here\n");
}


int main(){
    printf("runnning code tested here\n");
    check_run_time(testFunction1, "function one timing should take about 5 seconds");
    return 0;
}
