//USED GPT FOR EXAMPLE TO TEST
#include "raylib.h"
#include "../src/raymath.h"
#include <stdlib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "hash.c"
#include <time.h>


void pt_to_grid_plane(float x, float y, Color c){
    //DrawPlane(Vec, Vector2 size, Color color)
    if(x != 0 && y!=0){
    DrawPlane((Vector3){x-0.5f,0,y-0.5f}, (Vector2){1.0f,1.0f}, c);
    }
}


int main(void)
{
    float lastUpdate = 0;
    float interval = 10; //500 ms update
    float now;
    InitWindow(1200, 800, "Gyro-example with plane addtion");
    Camera cam = {0};
    //3D setup copied from RAYLIB
    cam.position = (Vector3){5.0, 5.0, 5.0};
    cam.target  =  (Vector3){0.0, 0.0, 0.0};
    cam.up = (Vector3){0.0, 1.0, 0.0};
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;


    //loading modles
    Model fish = LoadModel("/Users/tyler/Documents/fall2025/ECE4910/SeniorProject/CodeUser/obj/Fish.obj");
    Model Spider = LoadModel("/Users/tyler/Documents/fall2025/ECE4910/SeniorProject/CodeUser/obj/Spider.obj");
    Model Bike = LoadModel("/Users/tyler/Documents/fall2025/ECE4910/SeniorProject/CodeUser/obj/Bike.obj");
    Model q;
    Model items[3] = {fish, Spider, Bike};
    //file checking
    const char* basePath = GetWorkingDirectory();
    printf("Raylib working directory: %s\n", basePath);
    if(!FileExists("/Users/tyler/Documents/fall2025/ECE4910/SeniorProject/CodeUser/obj/fish.obj")){
        printf("FAILURE TO READ FILE :(\n");
    }



    int index = 0;
    bool showBox = false;
    bool boxTest = true;
    SetTargetFPS(45); //set to 45 by default.
    Vector3 pos =  {0, 0, 0};
    int i = 0;
    float angle = 0.0f;
    float r = 1.0f;
    float rotationSpeed = 0.05f;
    float omega = rotationSpeed / r * RAD2DEG;

    Color c = {100, 0, 0, 50};
    while (!WindowShouldClose())
    {
        UpdateCamera(&cam, CAMERA_FREE);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (GuiButton((Rectangle){ 0, 1, 200, 200 }, "Change the char")){
            showBox = !(showBox);
            index +=1;
            //printf("the bool %i %c \n", boxTest, *file);
            if(index > 3){
                index = 0;
            }
        }

        if (showBox)
            if (GuiMessageBox((Rectangle){ 0, 1, 200, 200 }, "MEH!", "Dis ist Spuder", "YA?")){
                index = 1;
                //printf("the bool %i %c \n", boxTest, *file);
            }
        int q = -1;
        srand(time(NULL));
        Bike.transform = MatrixRotateXYZ((Vector3){ DEG2RAD*rand(), DEG2RAD*0, DEG2RAD*0});

        /*3D portion of code */
        BeginMode3D(cam);
            float dt = GetFrameTime();
            float now = GetTime();
            angle += omega * dt;
            //here is the 3D action of stuff
            //DrawModel(items[index], (Vector3){0,0,0}, 0.03f, GREEN);
            //DrawModelWires(items[index], (Vector3){0,0,0}, 0.03f, BLACK);
            DrawPlane((Vector3){0.5f,0,0.5f}, (Vector2){1.0f,1.0f}, ORANGE);
            DrawPlane((Vector3){-0.5f,0,-0.5f}, (Vector2){1.0f,1.0f}, ORANGE);
            DrawPlane((Vector3){7.5f,0,-7.5f}, (Vector2){1.0f,1.0f}, ORANGE);
             DrawPlane((Vector3){-7.5f,0,-7.5f}, (Vector2){1.0f,1.0f}, ORANGE);
            DrawPlane((Vector3){1.5f,0,1.5f}, (Vector2){1.0f,1.0f}, c);
            q= q * -1;
            Color e = {255 % rand(), 0, 0, 255%rand()};
            pt_to_grid_plane((rand() % 11) , (rand() % 11)*-1, e);
            DrawGrid(20, 1.0f);
            //Trying this out to see what happens when I scale
            items[index].transform = MatrixScale(1, 0, 1);
            items[index].transform = MatrixRotate((Vector3){0,0,0}, i);
            Bike.transform = MatrixRotateXYZ((Vector3){ DEG2RAD*rand(), DEG2RAD*0, DEG2RAD*0});
            //points are refered to as x,y,z in raylib.
            DrawModelEx(items[index], (Vector3){0, 0, 0}, (Vector3){0,1,0}, angle, (Vector3){0.03f,0.03f,0.03f}, RED);
            DrawModelWiresEx(items[index], (Vector3){0,0,0}, (Vector3){1,0,0}, angle, (Vector3){0.03f,0.03f,0.03f}, BLACK);
            DrawModelWiresEx(items[index], (Vector3){0,0,0}, (Vector3){0,0,1}, angle, (Vector3){0.03f,0.03f,0.03f}, BLACK);
            DrawModelWires(Bike, (Vector3){5.0f,0.0f,0.0f}, 0.03f, BLACK);
            if(now - lastUpdate >= interval){
                lastUpdate = now;
                printf("Tick\n");
            }
            //printf("dt is %f the value of lastupdate is %f \n", dt, lastUpdate);
        EndMode3D();
        /*End of 3D portion of code */
        EndDrawing();

        if (angle > 360){
            angle = 0;
          }
    }

    CloseWindow();
    return 0;
}
