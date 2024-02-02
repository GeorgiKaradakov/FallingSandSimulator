#include <iostream>
#include <cmath>
#include <vector>
#include <SDL2/SDL.h>

#include "global_vars.h"
#include "user_def_functions.h"
#include "tile.h"
using namespace std;

int main(int argc, char **argv){
    srand(time(NULL));

    if(SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Unable to initialize SDL");
        return 1;
    }

    SDL_Window *win=SDL_CreateWindow("Falling Sand Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if(win==NULL){
        SDL_Log("Cannot create a window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Event e;
    SDL_Renderer *rend=SDL_CreateRenderer(win, -1, 0);

    std::vector<std::vector<tile>> tile_map;
    fill_tiles(tile_map);

    vector<SDL_Color> gradient_colors=get_gradients();
    vector<vector<tile>> new_tile_map;
    bool should_close=0, space_pressed=0;

    while(!should_close){
        fill_tiles(new_tile_map);//reset the secondary canvas

        proccess_events(tile_map, gradient_colors, e, should_close, space_pressed);
        if(space_pressed){
            tile_map.clear();
            fill_tiles(tile_map);
            continue;
        }

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);

        tile_move(tile_map, new_tile_map);
        tile_draw(rend, tile_map);

        SDL_RenderPresent(rend);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}