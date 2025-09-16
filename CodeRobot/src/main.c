#include <stdio.h>
#include <stdlib.h>
#include <sys/_select.h>
#include <sys/_types/_fd_def.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>
#include "read.c"
#include "action.c"
#include "cord.c"
#include "beg.c"
#include "read.c"

//todo implement functions here.
void customMode(){

}
void mapping(){

}

void cleaning(){

}

//main function
int main(){
    if(beginLoop() == 1){
        customMode();
    }
    else{

    }
    return 0;
}
