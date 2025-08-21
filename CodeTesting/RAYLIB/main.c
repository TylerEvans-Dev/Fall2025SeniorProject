#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "raylib.h"
#include "rlgl.h"
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
    int MAX_GRID_VAL = slices/2;

    //this is for the robot dim
    //DrawCylinder(Vector3 position x,y,z, float radiusTop, float radiusBottom, float height, int slices, Color color)
    const int rad = 1.0f;
    const int heo = 1.0f;
    const int slice = 30;

    //for the cube that is an object it will be red for edge
    const int ch = 1.0f;
    const int cw = 1.0f;
    const int cl = 1.0f;

    //this is for the postion of the robot
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    //this is for the angle of the robot
    float alpha = 0.0f;
    float beta = 0.0f;
    float gamma = 0.0f;

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

    //completly optional add extra stuff here.
    //DisableCursor();


    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        cam.target = (Vector3){x, y, z};
        cam.position = (Vector3){x+10, y+10, z+10};
        if(IsKeyPressed(KEY_UP) || IsKeyDown(KEY_UP)){
            z -= 0.3f;
        }
        if(IsKeyPressed(KEY_DOWN) ||IsKeyDown(KEY_DOWN)){
            z += 0.3f;
        }
        if(IsKeyPressed(KEY_LEFT) || IsKeyDown(KEY_LEFT)){
            x -= 0.3f;
        }
        if(IsKeyPressed(KEY_RIGHT) || IsKeyDown(KEY_RIGHT)){
            x += 0.3f;
        }
        if((fabs(x) > MAX_GRID_VAL) || (fabs(z) > MAX_GRID_VAL)){
            slices += slices;
            MAX_GRID_VAL = slices / 2;
        }


        ClearBackground(WHITE);
        UpdateCamera(&cam, CAMERA_FREE);

        BeginMode3D(cam);
        DrawCylinder((Vector3){x, y, z}, rad, rad, heo, slice, ORANGE);

        DrawCylinderWires((Vector3){x, y, z}, rad, rad, heo, slice, BLACK);

        if(IsKeyPressed(KEY_SPACE)){
            DrawCube((Vector3){x+rad,y,z+rad}, cw, ch, cl, RED);
        }

        DrawGrid(slices, spacing);

        EndMode3D();

        BeginDrawing();
        //drawing the window
        DrawRectangle(10, 10, 400, 200, BLUE);
        //rlRotatef(float angle, float x, float y, float z)

        DrawText("Test Window: controls", 10, 10, 20, BLACK);
        DrawText("w a s d move the view", 10, 30, 20, BLACK);
        DrawText("mouse change camera angle", 10, 50, 20, BLACK);
        DrawText("arrow keys to move the robot", 10, 70, 20, BLACK);
        DrawText(TextFormat("X: %f Y: %f, Z: %f", x,y,z), 10, 90, 20, BLACK);
        EndDrawing();

    }
    CloseWindow();
    return 0;
}
