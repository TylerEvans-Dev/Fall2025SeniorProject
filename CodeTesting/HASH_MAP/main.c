#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hash.c"

// ----- Example Program -----
int main(void) {
    Grid *grid = createGrid();

    // set a few cells (these could be in meters*100 or just arbitrary units)
    setCell(grid, -1, 2, CELL_OCCUPIED);
    setCell(grid, 1, -2, CELL_CLEAN);
    setCell(grid, 0, 0, CELL_CLEAN);
    setCell(grid, -3,-4, CELL_OCCUPIED);

    printf("Cell (-1, 2): %d\n", getCell(grid, -1, 2));
    printf("Cell (1, -2): %d\n", getCell(grid, 1, -2));
    printf("Cell (0, 0): %d\n", getCell(grid, 0, 0));

    // save map to file
    saveGrid(grid, "hash_map.txt");

    printf("\n--- Iterating over all cells ---\n");
    printAllCells(grid);

    // load into a new grid
    Grid *grid2 = createGrid();
    //loadGrid(grid2, "hash_map.txt");
    loadGridToClean(grid2, "hash_map.txt");

    printf("\n--- Loaded grid ---\n");
    printAllCells(grid2);

    freeGrid(grid);
    freeGrid(grid2);
    for(int i= 0; i ;i++){
        printf("the value of I \n");
    }
    return 0;
}
