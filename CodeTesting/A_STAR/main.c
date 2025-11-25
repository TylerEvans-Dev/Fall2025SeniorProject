#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAP_W 10
#define MAP_H 10

int grid[MAP_H][MAP_W] = {
    {0,0,0,0,0,0,0,0,1,0},
    {0,1,1,1,0,0,0,0,1,0},
    {0,0,0,1,0,0,0,0,1,0},
    {0,0,0,1,0,0,0,0,1,0},
    {0,0,0,1,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,1,1,0},
    {0,0,0,1,0,1,0,0,1,0},
    {0,0,0,1,0,1,0,0,1,0},
    {0,0,0,0,0,1,0,0,1,0},
    {0,1,1,1,0,0,0,0,0,0}
};

// Node structure
typedef struct Node {
    int x, y;
    float g;     // cost from start
    float h;     // heuristic
    float f;     // g + h
    struct Node* parent;
} Node;

// Simple priority-queue for open list
typedef struct {
    Node* items[MAP_W * MAP_H];
    int count;
} PriorityQueue;

void pq_push(PriorityQueue* pq, Node* n) {
    pq->items[pq->count++] = n;
}

Node* pq_pop(PriorityQueue* pq) {
    int best = 0;
    for (int i = 1; i < pq->count; i++)
        if (pq->items[i]->f < pq->items[best]->f)
            best = i;

    Node* n = pq->items[best];
    pq->items[best] = pq->items[--pq->count];
    return n;
}

int in_bounds(int x, int y) {
    return x >= 0 && y >= 0 && x < MAP_W && y < MAP_H;
}

float heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2); // Manhattan
}

int closed[MAP_H][MAP_W] = {0};

Node* nodes[MAP_H][MAP_W]; // store node pointers

Node* astar(int sx, int sy, int gx, int gy) {
    PriorityQueue open = {0};

    // Initialize start node
    Node* start = malloc(sizeof(Node));
    start->x = sx;
    start->y = sy;
    start->g = 0;
    start->h = heuristic(sx, sy, gx, gy);
    start->f = start->g + start->h;
    start->parent = NULL;

    nodes[sy][sx] = start;
    pq_push(&open, start);

    while (open.count > 0) {
        Node* current = pq_pop(&open);

        if (current->x == gx && current->y == gy) {
            return current; // goal reached!
        }

        closed[current->y][current->x] = 1;

        // Directions: up, down, left, right
        int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

        for (int i = 0; i < 4; i++) {
            int nx = current->x + dirs[i][0];
            int ny = current->y + dirs[i][1];

            if (!in_bounds(nx, ny)) continue;
            if (grid[ny][nx] == 1) continue; // blocked
            if (closed[ny][nx]) continue;

            float newG = current->g + 1;

            Node* neighbor = nodes[ny][nx];
            if (!neighbor) {
                neighbor = malloc(sizeof(Node));
                nodes[ny][nx] = neighbor;
                neighbor->x = nx;
                neighbor->y = ny;
                neighbor->g = newG;
                neighbor->h = heuristic(nx, ny, gx, gy);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = current;
                pq_push(&open, neighbor);
            }
            else if (newG < neighbor->g) {
                // Found a better path
                neighbor->g = newG;
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = current;
            }
        }
    }

    return NULL; // no path
}

void print_path(Node* goal) {
    Node* p = goal;
    while (p) {
        grid[p->y][p->x] = 2; // mark path
        p = p->parent;
    }

    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            if (grid[y][x] == 1) printf("█");
            else if (grid[y][x] == 2) printf("*");
            else printf(".");
        }
        printf("\n");
    }
}

int main() {
    Node* goal = astar(0, 0, 9, 9);

    if (goal)
        print_path(goal);
    else
        printf("No path found.\n");

    return 0;
}
