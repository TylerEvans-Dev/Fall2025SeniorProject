// A* implementation using your hash-map grid structure
// Visualizes each step by printing the grid state
// (Text-only visualization; integrate with raylib later if desired)

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "hash.c"  // <-- rename your provided code to a header or paste above

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
