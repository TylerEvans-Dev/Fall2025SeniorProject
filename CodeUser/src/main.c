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
//a global vector for use
Vector3 G;

// Make sure this matches the sender exactly (packed, same order)
typedef struct __attribute__((packed)) {
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
    float temp;
} DataAccel;

typedef struct Node{
    int x;
    int y;
    int h; //herustic
    int f;  //g+ h
}Node;

void pt_to_grid_plane(int x,int y, CellState s){
        if(s == CELL_UNKNOWN){
            DrawPlane((Vector3){x-0.5f, 0, y-0.5f}, (Vector2){1.0f, 1.0f}, GREEN);
        }
        else if(s == CELL_DIRTY){
            DrawPlane((Vector3){x-0.5f, 0, y-0.5f}, (Vector2){1.0f, 1.0f}, ORANGE);
            G = (Vector3){x,y-0.5};
        }
        else if(s == CELL_CLEAN){
            DrawPlane((Vector3){x-0.5f, 0, y-0.5f}, (Vector2){1.0f, 1.0f}, PURPLE);

        }
        else{
            DrawPlane((Vector3){x-0.5f, 0, y-0.5f}, (Vector2){1.0f, 1.0f}, GRAY);
        }
}
void loadAllCells(Grid *g) {
    for (int i = 0; i < HASH_SIZE; i++) {
        for (Cell *c = g->buckets[i]; c; c = c->next) {
            pt_to_grid_plane(c->x, c->y, c->state);
        }
    }
}
// void startTile(Vector3 o1) {
//     for (int i = 0; i < HASH_SIZE; i++) {
//         for (Cell *c = g->buckets[i]; c; c = c->next) {
//             printf("Cell (%d, %d): %d\n", c->x, c->y, c->state);
//         }
//     }
// }
int main(void)
{
    // // --- Networking setup ---
    // int server_fd, client_fd;
    // struct sockaddr_in addr;

    // server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // if (server_fd < 0) {
    //     perror("socket");
    //     return 1;
    // }

    // addr.sin_family = AF_INET;
    // addr.sin_addr.s_addr = INADDR_ANY;
    // addr.sin_port = htons(9000);

    // if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    //     perror("bind");
    //     close(server_fd);
    //     return 1;
    // }
    // listen(server_fd, 1);

    // printf("Waiting for connection...\n");
    // client_fd = accept(server_fd, NULL, NULL);
    // if (client_fd < 0) {
    //     perror("accept");
    //     close(server_fd);
    //     return 1;
    // }
    // printf("Client connected!\n");

    // --- Raylib / scene setup ---
    float lastUpdate = 0.0f;
    const float interval = 0.05f; // read every 50ms (adjust as needed)
    InitWindow(1200, 800, "Gyro-example with plane addition");
    Camera cam = {0};
    cam.position = (Vector3){5.0f, 5.0f, 5.0f};
    cam.target   = (Vector3){0.0f, 0.0f, 0.0f};
    cam.up       = (Vector3){0.0f, 1.0f, 0.0f};
    cam.fovy     = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    // loading models (keep your paths)
    Model fish = LoadModel("/Users/tyler/Documents/fall2025/ECE4910/SeniorProject/CodeUser/obj/Fish.obj");
    Model Spider = LoadModel("/Users/tyler/Documents/fall2025/ECE4910/SeniorProject/CodeUser/obj/Spider.obj");
    Model Bike = LoadModel("/Users/tyler/Documents/fall2025/ECE4910/SeniorProject/CodeUser/obj/Bike.obj");

    const char* basePath = GetWorkingDirectory();
    printf("Raylib working directory: %s\n", basePath);
    if(!FileExists("/Users/tyler/Documents/fall2025/ECE4910/SeniorProject/CodeUser/obj/fish.obj")){
        printf("FAILURE TO READ FILE :(\n");
    }

    DataAccel d = {0};
    // angles in degrees



    Grid* grid = createGrid();
    loadGrid(grid, "/Users/tyler/Documents/fall2025/ECE4910/SeniorProject/CodeUser/src/map3.txt");
    printAllCells(grid);

    SetTargetFPS(60);
    srand((unsigned)time(NULL)); // seed once
    printf("the value of g %f, %f %f \n", G.x, G.y, G.z);
    while (!WindowShouldClose())
    {
        UpdateCamera(&cam, CAMERA_FREE);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Simple GUI example (unchanged)
        if (GuiButton((Rectangle){ 0, 1, 200, 200 }, "Change the char")){
            // ...
        }

        BeginMode3D(cam);

        // Timing
        float dt = GetFrameTime();           // seconds
        double now = GetTime();              // double time in seconds

        DrawGrid(10000, 1.0f);
        //DrawModelWires(Bike, (Vector3){0.0f, 0.0f, 0.0f}, 1, BLACK);
        //DrawCircle3D(, float radius, Vector3 rotationAxis, float rotationAngle, Color color)
        //DrawCube(G, (Vector3){1.0f, 1.0f, 1.0f}, BLUE);
        //DrawCube(G, 1.0f, 1.0f, 1.0f, BLUE);
        //DrawCubeWires(G, 1.0f, 1.0f, 1.0f, BLACK);
        loadAllCells(grid);
        DrawCube((Vector3){G.x,0.5f,G.y}, 1.0f, 1.0f, 1.0f, RED);
        EndMode3D();
        EndDrawing();
    }

    //close(client_fd);
    //close(server_fd);
    freeGrid(grid);
    CloseWindow();
    return 0;
}
