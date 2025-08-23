#include "raylib.h"
#include "math.h"
// Translates object to a give spot on grid
void move(float *x, float *y, float *z, int *slices, int *w){
    if(IsKeyPressed(KEY_UP) || IsKeyDown(KEY_UP)){
        *z -= 0.3f;
    }
    if(IsKeyPressed(KEY_DOWN) ||IsKeyDown(KEY_DOWN)){
        *z += 0.3f;
    }
    if(IsKeyPressed(KEY_LEFT) || IsKeyDown(KEY_LEFT)){
        *x -= 0.3f;
    }
    if(IsKeyPressed(KEY_RIGHT) || IsKeyDown(KEY_RIGHT)){
        *x += 0.3f;
    }
    if((fabs(*x) > *w) || (fabs(*z) > *w)){
        *slices += *slices;
        *w = *slices / 2;
    }
}
