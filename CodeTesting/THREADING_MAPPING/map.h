#include <cstdint>
#define map_H
#ifdef map_H
/*
 * Tyler Evans U1313811 20.10 -
 * Mapping and location
 * tracking built with the help of gpt and deepseek,
 * sources 5 mins details https://www.youtube.com/watch?v=8ckhPViqneg&t=35s
 */




typedef struct{
  float x;
  float y;
  float z;
} point;

/*
 * @struct robot_location
 * @param x,y,z postion
 * @param phi about x axis,theta z axis, psy angleabout y axis of robot
 */
typedef struct{
    //location using x,y,z cartisan
    float x;
    float y;
    float z;
    //euler angles
    float phi;
    float theta;
    float psy;
}robot_location;


typedef struct{
    int width;
    int height;
    float resolution;
    int origin_x;
    int origin_y;
    uint8_t *data;

} grid;


typedef struct{
    int sessionID;
    grid maps[4];
} tabletop_grid_collections;

void get_x_y(int x, int y);
void set_data(int x, int y);
void find_path();
    //TODO find path depending on the di
//done at the end of every mapping session
void savemap(grid *g);
    //TODO we are going to save map

/*
 * function to load the map for the robot
 *
 */
void loadmap();
//TODO here there nees to be a function to load the maps from memory

#endif
