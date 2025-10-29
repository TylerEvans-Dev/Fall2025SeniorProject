//USED GPT FOR EXAMPLE TO TEST
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(void)
{
    InitWindow(800, 450, "raylib + raygui (CMake local build)");

    bool showBox = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (GuiButton((Rectangle){ 350, 200, 100, 40 }, "Toggle"))
            showBox = !showBox;

        if (showBox)
            GuiMessageBox((Rectangle){ 300, 150, 200, 100 }, "Hello", "This is raygui", "OK");

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
