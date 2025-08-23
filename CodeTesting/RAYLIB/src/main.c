#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "rlgl.h"
#include "move.c"
//Tyler Evans U1313811 this is an example program for

int main(){
    // Height and width of the window.
    const int h = 1000;
    const int w = 900;
    // This is for the 1.0f for 1cm refrence
    // used for the draw grid window
    // DrawGrid(int slices, float spacing)
    int slices = 10;
    float spacing = 1.0f;
    int MAX_GRID_VAL = slices / 2;

    //this is for the robot dim
    //DrawCylinder(Vector3 position x,y,z, float radiusTop, float radiusBottom, float height, int slices, Color color)
    const float rad = 1.0f;
    const float heo = 1.0f;
    const int slice = 30;

    //for the cube that is an object it will be red for edge
    const float ch = 1.0f;
    const float cw = 5.0f;
    const float cl = 5.0f;

    //this is for the postion of the robot
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    //this is for the angle of the robot
    float alpha = 0.0f;
    float beta = 0.0f;
    float gamma = 0.0f;
    //battery of the battery value.
    float batVal = 100.0f;

    InitWindow(h, w, "First screen");

    //define camera
    Camera3D cam= {0};
    //give it a starting postion
    // this is an inline definition here Vector3 is a struct and the f defines the values behind it :)
    cam.position = (Vector3){10.0f, 10.0f, 10.0f};
    //this is the target of the object where it will be pointing to
    cam.target = (Vector3){0.0f, 0.0f, 0.0f};
    // this is where the camera is pointing towards
    cam.up = (Vector3){0.0f, 1.0f, 0.0f};
    //this is the camera apeature or where it is working
    cam.fovy = (50.0f);
    //this sets the camera mode
    cam.projection = CAMERA_PERSPECTIVE;

    //for battery
    Vector2 spherePosition = {0.0f, 0.0f};
    //Edge drawing
    int size = 50;
    int items = 0;
    int index = 0;
    struct Vector3 arr[size];

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        cam.target = (Vector3){x, y, z};
        cam.position = (Vector3){x+10, y+10, z+10};
        move(&x, &y, &z, &slices, &MAX_GRID_VAL);

        ClearBackground(WHITE);
        UpdateCamera(&cam, CAMERA_FREE);

        BeginMode3D(cam);
        //this is for drawing the vaccume
        DrawCylinder((Vector3){x, y, z}, rad, rad, heo, slice, ORANGE);
        DrawCylinderWires((Vector3){x, y, z}, rad, rad, heo, slice, BLACK);
        spherePosition = GetWorldToScreen((Vector3){x,y,z}, cam);

        if(IsKeyPressed(KEY_SPACE)){
            if(items < size){
                arr[index]  = (Vector3){5+x,y,z};
                index +=1;
                items += 1;
            }
            if(size == index){
                printf("no more can be added\n");
            }
        }
        if(IsKeyPressed(KEY_Z)){
            if(items < size){
                arr[index]  = (Vector3){((x-5)),y,z};
                index +=1;
                items += 1;
            }
            if(size == index){
                printf("no more can be added\n");
            }
        }
        if(IsKeyPressed(KEY_Q)){
            if(items < size){
                arr[index]  = (Vector3){x,y,((z-5))};
                index +=1;
                items += 1;
            }
            if(size == index){
                printf("no more can be added\n");
            }
        }
        if(IsKeyPressed(KEY_E)){
            if(items < size){
                arr[index]  = (Vector3){x,y,z+5};
                index +=1;
                items += 1;
            }
            if(size == index){
                printf("no more can be added\n");
            }
        }
        //draws the edges
        if(items != 0){
            for(int in = 0; in < items; in++){
                DrawCube(arr[in], cw, ch, cl, RED);
                DrawCube(arr[in], cw, ch, cl, MAROON);
            }
        }

        DrawGrid(slices, spacing);

        EndMode3D();

        BeginDrawing();
        //drawing the window
        DrawRectangle(10, 10, 400, 200, BLUE);


        DrawText("Test Window: controls", 10, 10, 20, BLACK);
        DrawText("w a s d move the view", 10, 30, 20, BLACK);
        DrawText("mouse change camera angle", 10, 50, 20, BLACK);
        DrawText("arrow keys to move the robot", 10, 70, 20, BLACK);
        DrawText(TextFormat("X: %f Y: %f, Z: %f", x,y,z), 10, 90, 20, BLACK);
        DrawText(TextFormat("Battery %f %", batVal), spherePosition.x-10, spherePosition.y+30, 30, BLACK);
        EndDrawing();

    }
    CloseWindow();
    return 0;
}
