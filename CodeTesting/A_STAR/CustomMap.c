#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hash.c"

int main(void) {
    const int size = 100;
    const int half = size / 2;

    srand((unsigned)time(NULL));
    Grid *grid = createGrid(); //creates a grid
    for (int y = -half; y < half; y++) {
        for (int x = -half; x < half; x++) {
            int value = (rand() % 4) + 1;
            printf("%d %d %d\n", x, y, value);
            setCell(grid, x, y, value);
        }
    }
    saveGrid(grid,"practice_map.txt");
    freeGrid(grid);
    return 0;
}
