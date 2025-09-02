#include "raylib.h"
#include <stdio.h>
void addFront(Vector3* o){

}

void addLeft(Vector3* o){

}

void addRight(Vector3* o){

}

void addBack(Vector3 *o, int* item, int* index, int* size, float x, float y, float z){
    if(*item < *size){
        o[*index]  = (Vector3){x,y,((z-5))};
        *index +=1;
        *item += 1;
    }
}
