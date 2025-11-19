//USED GPT FOR EXAMPLE TO TEST
#include "raylib.h"
#include "../src/raymath.h"
#include <stdlib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "hash.c"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


void pt_to_grid_plane(float x, float y, Color c){
    //DrawPlane(Vec, Vector2 size, Color color)
    if(x != 0 && y!=0){
    DrawPlane((Vector3){x-0.5f,0,y-0.5f}, (Vector2){1.0f,1.0f}, c);
    }
}


int main(void)
{
    //used for comunication

    typedef struct{
        float ax;
        float ay;
        float az;
        float gx;
        float gy;
        float gz;
        float temp;
    }DataAccel;



    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(9000);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf("Waiting for connection...\n");
    client_fd = accept(server_fd, NULL, NULL);
    printf("Client connected!\n");
    //
    //end of section used for comunication
    float lastUpdate = 0;
    float interval = 0.5; //500 ms update
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

    DataAccel d;

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
        srand(time(NULL));
        //Bike.transform = MatrixRotateXYZ((Vector3){ DEG2RAD*0, DEG2RAD*0, DEG2RAD*0});

        /*3D portion of code */
        BeginMode3D(cam);
            float dt = GetFrameTime();
            float time = GetTime();
            angle += omega * dt; // calcualted via ang
            //here is the 3D action of stuff
            DrawGrid(20, 1.0f); //draw a grid according to the size of the map.
            //Trying this out to see what happens when I scale
            //Bike.transform = MatrixRotateXYZ((Vector3){ DEG2RAD*200*d.gx, DEG2RAD*200*d.gy, DEG2RAD*200*d.gz});
            DrawModelWires(Bike, (Vector3){0.0f,-5.0f,0.0f}, 0.08f, BLACK);
            if(time - lastUpdate >= interval){

                recv(client_fd, &d, sizeof(DataAccel)*7, MSG_DONTWAIT);
                printf("gx=%f gy=%f gz=%f\n", d.gx, d.gy, d.gz);
                lastUpdate = time;
            }
             Bike.transform = MatrixRotateXYZ((Vector3){ DEG2RAD*d.gx, DEG2RAD*d.gy, 0});
            //printf("dt is %f the value of lastupdate is %f \n", dt, lastUpdate);
        EndMode3D();
        /*End of 3D portion of code */
        EndDrawing();

        if (angle > 360){
            angle = 0;
          }
    }
    close(client_fd);
    close(server_fd);

    CloseWindow();
    return 0;
}
