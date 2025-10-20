#include <pthread.h>
#include <stdio.h>
//accel. for reading a sensor for one thread
//#include "GY521_MPU/GY521.h"
#include <time.h>
#include <unistd.h>
#include "timing.c"

void testFunction1(){
    sleep(5);
    printf("function 1 \n");
}

void testFunction2(){
    usleep(2000000);
    printf("function 2 here\n");
}

void testFunction3(){
    struct timespec requested_time, remaining_time;
    requested_time.tv_nsec = 500000000;
    requested_time.tv_sec = 0;
    nanosleep(&requested_time, &remaining_time);
}
void runtest(int run){
    if(run ==1){
    printf("runnning code tested here\n");
    check_run_time(testFunction1, "test function 1 should take about 5 seconds");
    check_run_time(testFunction2, "test function 2 should be 10 seconds about ");
    check_run_time(testFunction3, "test function 3 should be 500ms about ");
    }
}


int main(){
    //for testing timing
    runtest(0);

    return 0;
}
