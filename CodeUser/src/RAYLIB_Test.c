// Minimal Raylib A* Visualization Example
// ----------------------------------------
// This version shows the A* algorithm running visually on a grid.
// Click to place start/end points; press SPACE to step the algorithm.
// Integrate with your hashmap grid by replacing get/set logic.

#include "raylib.h"
#include <stdlib.h>
#include <math.h>

#define GRID_W 40
#define GRID_H 30
#define CELL 20

// Cell states
typedef enum {
    FREE = 0,
    WALL = 1,
    OPEN = 2,
    CLOSED = 3,
    START = 4,
    END = 5,
    PATH = 6
} CellState;

// Simple grid
CellState grid[GRID_H][GRID_W];

// A* node
typedef struct Node {
    int x, y;
    int g, h, f;
    struct Node *parent;
    struct Node *next;
} Node;

Node *openList = NULL;
Node *current = NULL;
Node *goalNode = NULL;
int startX = -1, startY = -1;
int endX = -1, endY = -1;

// PQ insert
Node *pushPQ(Node *head, Node *n) {
    if (!head || n->f < head->f) {
        n->next = head;
        return n;
    }
    Node *cur = head;
    while (cur->next && cur->next->f <= n->f)
        cur = cur->next;
    n->next = cur->next;
    cur->next = n;
    return head;
}

Node *popPQ(Node **head) {
    if (!*head) return NULL;
    Node *n = *head;
    *head = (*head)->next;
    return n;
}

int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

void resetAlgo() {
    Node *n;
    while (openList) {
        n = popPQ(&openList);
        free(n);
    }
    openList = NULL;
    current = NULL;
    goalNode = NULL;

    for (int y = 0; y < GRID_H; y++)
        for (int x = 0; x < GRID_W; x++)
            if (grid[y][x] != WALL)
                grid[y][x] = FREE;

    if (startX >= 0) grid[startY][startX] = START;
    if (endX >= 0) grid[endY][endX] = END;
}

void beginAStar() {
    resetAlgo();
    if (startX < 0 || endX < 0) return;

    Node *s = calloc(1, sizeof(Node));
    s->x = startX;
    s->y = startY;
    s->g = 0;
    s->h = heuristic(startX, startY, endX, endY);
    s->f = s->h;
    openList = pushPQ(openList, s);
}

void stepAStar() {
    if (!openList || goalNode) return;

    current = popPQ(&openList);
    int cx = current->x;
    int cy = current->y;

    if (grid[cy][cx] != START && grid[cy][cx] != END)
        grid[cy][cx] = CLOSED;

    if (cx == endX && cy == endY) {
        goalNode = current;
        Node *p = goalNode->parent;
        while (p && !(p->x == startX && p->y == startY)) {
            grid[p->y][p->x] = PATH;
            p = p->parent;
        }
        return;
    }

    int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

    for (int i = 0; i < 4; i++) {
        int nx = cx + dirs[i][0];
        int ny = cy + dirs[i][1];
        if (nx < 0 || ny < 0 || nx >= GRID_W || ny >= GRID_H) continue;
        if (grid[ny][nx] == WALL || grid[ny][nx] == CLOSED) continue;

        Node *n = calloc(1, sizeof(Node));
        n->x = nx;
        n->y = ny;
        n->g = current->g + 1;
        n->h = heuristic(nx, ny, endX, endY);
        n->f = n->g + n->h;
        n->parent = current;

        if (grid[ny][nx] == FREE)
            grid[ny][nx] = OPEN;

        openList = pushPQ(openList, n);
    }
}

Color cellColor(CellState c) {
    switch (c) {
        case FREE: return RAYWHITE;
        case WALL: return DARKGRAY;
        case OPEN: return SKYBLUE;
        case CLOSED: return BLUE;
        case START: return GREEN;
        case END: return RED;
        case PATH: return YELLOW;
    }
    return BLACK;
}

int main() {
    InitWindow(GRID_W * CELL, GRID_H * CELL, "A* Visualization");
    SetTargetFPS(60);

    // Fill empty
    for (int y = 0; y < GRID_H; y++)
        for (int x = 0; x < GRID_W; x++)
            grid[y][x] = FREE;

    while (!WindowShouldClose()) {
        // Click to place start
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int mx = GetMouseX() / CELL;
            int my = GetMouseY() / CELL;
            startX = mx;
            startY = my;
            grid[my][mx] = START;
        }
        // Right click to place end
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            int mx = GetMouseX() / CELL;
            int my = GetMouseY() / CELL;
            endX = mx;
            endY = my;
            grid[my][mx] = END;
        }
        // Middle click toggles wall
        if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
            int mx = GetMouseX() / CELL;
            int my = GetMouseY() / CELL;
            if (grid[my][mx] == FREE)
                grid[my][mx] = WALL;
        }

        // SPACE begins/steps the algorithm
        if (IsKeyPressed(KEY_SPACE)) {
            if (!openList && !goalNode)
                beginAStar();
            else
                stepAStar();
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (int y = 0; y < GRID_H; y++) {
            for (int x = 0; x < GRID_W; x++) {
                DrawRectangle(x * CELL, y * CELL, CELL - 1, CELL - 1, cellColor(grid[y][x]));
            }
        }

        DrawText("Left = Start, Right = End, Middle = Wall, SPACE = A* Step", 10, 10, 10, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// A* implementation using your hash-map grid structure
// Visualizes each step by printing the grid state
// (Text-only visualization; integrate with raylib later if desired)

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "hashmap.h"  // <-- rename your provided code to a header or paste above

// ---------------------------------------------
// Node structure for A*
// ---------------------------------------------
typedef struct Node {
    int x, y;
    int g;        // cost from start
    int h;        // heuristic
    int f;        // g + h
    struct Node *parent;
    struct Node *next;  // for priority queue
} Node;

// ---------------------------------------------
// Priority queue sorted by f-cost
// ---------------------------------------------
Node *pushPQ(Node *head, Node *n) {
    if (!head || n->f < head->f) {
        n->next = head;
        return n;
    }
    Node *cur = head;
    while (cur->next && cur->next->f <= n->f)
        cur = cur->next;
    n->next = cur->next;
    cur->next = n;
    return head;
}

Node *popPQ(Node **head) {
    if (!*head) return NULL;
    Node *n = *head;
    *head = (*head)->next;
    return n;
}

// Manhattan distance
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// ---------------------------------------------
// Visualization helper
// ---------------------------------------------
void visualizeStep(Grid *g, int sx, int sy, int ex, int ey, Node *open, int cx, int cy) {
    printf("===== A* STEP =====\n");
    printf("Current: (%d,%d)\n", cx, cy);

    // Show open list contents
    printf("Open list: ");
    for (Node *n = open; n; n = n->next)
        printf("(%d,%d,f=%d) ", n->x, n->y, n->f);
    printf("\n\n");

    // Scan a small neighborhood for text visualization
    for (int y = sy - 5; y <= sy + 5; y++) {
        for (int x = sx - 10; x <= sx + 10; x++) {
            if (x == sx && y == sy) { printf("S "); continue; }
            if (x == ex && y == ey) { printf("E "); continue; }
            if (x == cx && y == cy) { printf("C "); continue; }
            CellState s = getCell(g, x, y);
            switch(s) {
                case CELL_OCCUPIED: printf("# "); break;
                case CELL_FREE:     printf(". "); break;
                case CELL_CLEAN:    printf("C "); break;
                case CELL_DIRTY:    printf("D "); break;
                default:            printf("? "); break;
            }
        }
        printf("\n");
    }
    printf("====================\n\n");
}


// ---------------------------------------------
// A* Search
// ---------------------------------------------
int astar(Grid *g, int sx, int sy, int ex, int ey) {
    Node *open = NULL;
    int visited[200][200] = {0};   // adjust if using huge grid

    Node *start = calloc(1, sizeof(Node));
    start->x = sx;
    start->y = sy;
    start->g = 0;
    start->h = heuristic(sx, sy, ex, ey);
    start->f = start->h;
    open = pushPQ(open, start);

    while (open) {
        Node *cur = popPQ(&open);
        int cx = cur->x;
        int cy = cur->y;

        // visualize this step
        visualizeStep(g, sx, sy, ex, ey, open, cx, cy);

        if (cx == ex && cy == ey) {
            printf("Goal reached!\n");
            return 1;
        }

        visited[cx][cy] = 1;

        int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        for (int i = 0; i < 4; i++) {
            int nx = cx + dirs[i][0];
            int ny = cy + dirs[i][1];

            if (visited[nx][ny]) continue;
            CellState s = getCell(g, nx, ny);
            if (s == CELL_OCCUPIED) continue;

            Node *n = calloc(1, sizeof(Node));
            n->x = nx;
            n->y = ny;
            n->g = cur->g + 1;
            n->h = heuristic(nx, ny, ex, ey);
            n->f = n->g + n->h;
            n->parent = cur;

            open = pushPQ(open, n);
        }
    }

    printf("No path found.\n");
    return 0;
}


// ---------------------------------------------
// MAIN TEST
// ---------------------------------------------
int main() {
    Grid *g = createGrid();

    // Create a simple sample grid
    for (int y = 0; y < 20; y++)
        for (int x = 0; x < 20; x++)
            setCell(g, x, y, CELL_FREE);

    // Add a wall
    for (int x = 5; x < 15; x++)
        setCell(g, x, 10, CELL_OCCUPIED);

    astar(g, 2, 2, 18, 18);

    freeGrid(g);
    return 0;
}
