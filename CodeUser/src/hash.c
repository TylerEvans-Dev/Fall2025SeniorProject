#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define HASH_SIZE 10007 // number of buckets (prime number for even distribution)
#define DATA_PER_ROW 3
typedef enum {
    CELL_UNKNOWN = 0,
    CELL_FREE = 1,
    CELL_OCCUPIED = 2,
    CELL_CLEAN =3,
    CELL_DIRTY =4
} CellState;

// ----- Hash Function -----
// This is the hasing function that puts the data into the hash map.
static unsigned int hashXY(int x, int y) {
    // large primes reduce collisions
    unsigned int h = ((x * 73856093) ^ (y * 19349663));
    return h % HASH_SIZE;
}
// This is the structure of the cell so x,y and the state along with a ptr to the next cell.
typedef struct Cell {
    int x, y;
    int cost;
    int dist;
    CellState state;
    struct Cell *next;
} Cell;
//This is the grid structure data has buckets
typedef struct {
    Cell *buckets[HASH_SIZE];
} Grid;

// ----- Create and Free Grid -----
// This creates a grid
Grid *createGrid(void) {
    Grid *g = calloc(1, sizeof(Grid));
    return g;
}

void freeGrid(Grid *g) {
    //this is a mem. control function to free the data after use in ram.
    for (int i = 0; i < HASH_SIZE; i++) {
        Cell *c = g->buckets[i];
        while (c) {
            Cell *tmp = c;
            c = c->next;
            free(tmp);
        }
    }
    free(g);
}

// ----- Set and Get -----
void setCell(Grid *g, int x, int y, CellState state) {
    unsigned int h = hashXY(x, y);
    Cell *c = g->buckets[h];

    // check if cell already exists
    while (c) {
        if (c->x == x && c->y == y) {
            c->state = state;
            return;
        }
        c = c->next;
    }

    // create new cell
    Cell *newCell = malloc(sizeof(Cell));
    newCell->x = x;
    newCell->y = y;
    newCell->state = state;
    newCell->next = g->buckets[h];
    g->buckets[h] = newCell;
}

CellState getCell(Grid *g, int x, int y) {
    unsigned int h = hashXY(x, y);
    Cell *c = g->buckets[h];
    while (c) {
        if (c->x == x && c->y == y)
            return c->state;
        c = c->next;
    }
    return CELL_UNKNOWN;
}


// ----- Iteration Example -----
void printAllCells(Grid *g) {
    for (int i = 0; i < HASH_SIZE; i++) {
        for (Cell *c = g->buckets[i]; c; c = c->next) {
            printf("Cell (%d, %d): %d\n", c->x, c->y, c->state);
        }
    }
}


// ----- Save and Load -----
//saves a grid from file
void saveGrid(Grid *g, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;

    for (int i = 0; i < HASH_SIZE; i++) {
        for (Cell *c = g->buckets[i]; c; c = c->next) {
            fprintf(f, "%d %d %d\n", c->x, c->y, c->state);
        }
    }
    fclose(f);
}
//loads a grid from a file
void loadGrid(Grid *g, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;
    int x, y, s;
    while (fscanf(f, "%d %d %d", &x, &y, &s) == DATA_PER_ROW) {
        setCell(g, x, y, (CellState)s);
    }
    fclose(f);
}

void loadGridToClean(Grid *g, const char *filename){
    FILE *f = fopen(filename, "r");
    if(!f) return;
    int x;
    int y;
    int s;
    while(fscanf(f, "%d %d %d", &x, &y, &s) == DATA_PER_ROW){
        if(((CellState)s) == CELL_CLEAN){
            setCell(g, x, y, CELL_DIRTY);
        }
        else{
            setCell(g, x,y, (CellState)s);
        }
    }
}


void sendPoint(){

}
