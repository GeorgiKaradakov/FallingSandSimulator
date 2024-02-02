#pragma once
#include <vector>

#define TILE_SIZE 4  //TODO: optimize the code to work smoothly with TILE_SIZE=1
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WIDTH WINDOW_WIDTH/TILE_SIZE
#define HEIGHT WINDOW_HEIGHT/TILE_SIZE
#define RAD 6.5

#define TILES_PER_CLICK 6
#define COLORS_SIZE 7

int color_cnt=0, color_ind=0;