//USED GPT FOR EXAMPLE TO TEST
#include "raylib.h"
#include "../src/raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


int main(void)
{
    InitWindow(1200, 800, "Gyro-example with gui options");
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
    float rotationSpeed = 5.0f;
    float omega = rotationSpeed / r * RAD2DEG;
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

        /*3D portion of code */
        BeginMode3D(cam);
            float dt = GetFrameTime();
            angle += omega * dt;
            //here is the 3D action of stuff
            //DrawModel(items[index], (Vector3){0,0,0}, 0.03f, GREEN);
            //DrawModelWires(items[index], (Vector3){0,0,0}, 0.03f, BLACK);
            DrawPlane((Vector3){0.5f,0,0.5f}, (Vector2){1.0f,1.0f}, ORANGE);
            DrawGrid(20, 1.0f);
            //Trying this out to see what happens when I scale
            items[index].transform = MatrixScale(1, 0, 1);
            items[index].transform = MatrixRotate((Vector3){0,0,0}, i);
            //points are refered to as x,y,z in raylib.
            DrawModelEx(items[index], (Vector3){0, 0, 0}, (Vector3){0,1,0}, angle, (Vector3){0.03f,0.03f,0.03f}, RED);
            DrawModelWiresEx(items[index], (Vector3){0,0,0}, (Vector3){1,0,0}, angle, (Vector3){0.03f,0.03f,0.03f}, BLACK);
            DrawModelWiresEx(items[index], (Vector3){0,0,0}, (Vector3){0,0,1}, angle, (Vector3){0.03f,0.03f,0.03f}, BLACK);
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
