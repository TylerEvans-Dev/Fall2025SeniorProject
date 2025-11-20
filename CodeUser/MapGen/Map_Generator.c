#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../src/hash.c"

#define GRID_W 200
#define GRID_H 200

#define ROOMS 50
#define MIN_SIZE 30
#define MAX_SIZE 50

typedef struct {
    int x, y;      // Top-left corner (can be negative)
    int w, h;
    int cx, cy;    // Center for corridor linking
} Room;

Room rooms[ROOMS];

// Convert "world coords" (which may be negative) to grid indices
int ox(int x) { return x + GRID_W / 2; }
int oy(int y) { return y + GRID_H / 2; }

char grid[GRID_H][GRID_W];

// Carve a rectangular room into the grid
void carve_room(Room r) {
    for (int y = r.y; y < r.y + r.h; y++) {
        for (int x = r.x; x < r.x + r.w; x++) {
            int gx = ox(x);
            int gy = oy(y);
            if (gx >= 0 && gx < GRID_W && gy >= 0 && gy < GRID_H){
                grid[gy][gx] = '.'; // room floor
            }
        }
    }
}

void cleanMapEdges(){
    for (int y = 0; y < GRID_H; y++)
        for (int x = 0; x < GRID_W; x++)
            if(x == 0 || y == 0){
                grid[y][x] = '#';
            }

}

// Carve an L-shaped corridor between two room centers
void carve_corridor(int x1, int y1, int x2, int y2) {
    int gx, gy;

    // Horizontal section
    for (int x = (x1 < x2 ? x1 : x2); x <= (x1 < x2 ? x2 : x1); x++) {
        gx = ox(x);
        gy = oy(y1);
        if (gx >= 0 && gx < GRID_W && gy >= 0 && gy < GRID_H){
            grid[gy][gx] = '.';
            grid[gy-1][gx-1] = '.';
            grid[gy+1][gx+1] = '.';
        }
    }

    // Vertical section
    for (int y = (y1 < y2 ? y1 : y2); y <= (y1 < y2 ? y2 : y1); y++) {
        gx = ox(x2);
        gy = oy(y);
        if (gx >= 0 && gx < GRID_W && gy >= 0 && gy < GRID_H){
            grid[gy][gx] = '.';
            grid[gy-1][gx-1] = '.';
            grid[gy+1][gx+1] = '.';
        }
    }
}

// Connect rooms using a simple MST-like chaining
void connect_rooms() {
    for (int i = 0; i < ROOMS - 1; i++) {
        int x1 = rooms[i].cx;
        int y1 = rooms[i].cy;
        int x2 = rooms[i + 1].cx;
        int y2 = rooms[i + 1].cy;

        carve_corridor(x1, y1, x2, y2);
    }
}

int main() {
    Grid* gridc = createGrid();

    srand(time(NULL));

    // Initialize grid with walls
    for (int y = 0; y < GRID_H; y++)
        for (int x = 0; x < GRID_W; x++)
            grid[y][x] = '#';

    // Generate random rooms
    for (int i = 0; i < ROOMS; i++) {
        Room r;

        r.w = MIN_SIZE + rand() % (MAX_SIZE - MIN_SIZE + 1);
        r.h = MIN_SIZE + rand() % (MAX_SIZE - MIN_SIZE + 1);

        // Random position, can be negative
        r.x = (rand() % (GRID_W - r.w)) - GRID_W / 4;
        r.y = (rand() % (GRID_H - r.h)) - GRID_H / 4;

        r.cx = r.x + r.w / 2;
        r.cy = r.y + r.h / 2;

        rooms[i] = r;
        carve_room(r);
    }

    // Ensure connectivity with corridors
    connect_rooms();
    cleanMapEdges();
    // Print grid
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            putchar(grid[y][x]);
            if(grid[y][x] == '#'){
                setCell(gridc, x, y, CELL_OCCUPIED);
            }
            if(grid[y][x] == '.'){
                setCell(gridc, x, y, CELL_DIRTY);
            }

        }
        putchar('\n');
    }

    saveGrid(gridc, "map.txt");
    freeGrid(gridc);
    return 0;
}
