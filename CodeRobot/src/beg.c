#include <stdio.h>
#include <stdlib.h>
#include <sys/_select.h>
#include <sys/_types/_fd_def.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>
// Written by Tyler Evans and GPT 5
 #define TIME 15
int beginLoop(){
    int input;
    fd_set fds;
    struct timeval tv;
    int det = TIME;

    while(det > 0){
    printf("\033[2j \n");
    printf("enter 1 to go into custom mode \n");
    printf("time left to enter an option %i \n", det);

    //this func is for
    FD_ZERO(&fds); //creates an active listener for an event
    //this func is for
    FD_SET(STDIN_FILENO, &fds); //makes it see that there is input from the key board

    //this sets 1 sec.
    tv.tv_sec = 2;
    //this is the 0 milisecond
    tv.tv_usec = 0;
    int ready = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv); //select tells the system to listen for an event at the system level

    if (ready > 0) {
            if (scanf("%d", &input) == 1 && input == 1) {
                printf("Going into custom operation");
                return 1;
            } else {
                printf("received no input going into standard auto motion");
            }
        }
        printf("\033[2j \n");
        system("clear");
        det--;
    }
    return -1;
}
