#include <stdio.h>
#include "../HASH_MAP/hash.c"
#include <math.h>

typedef struct Node{
    int x, y;
    float g; //cost from start
    float h; // hueristic
    float f; // g+h
    struct Node* parent;
} Node;

typedef struct{

} PQ;

int main(){

}
