#pragma once
#include <SDL2/SDL.h>
#include <vector>

#include "global_vars.h"

struct tile{
    SDL_Rect rect;
    SDL_Color c;
    int state, speed;
};

void fill_tiles(std::vector<std::vector<tile>> &tiles){
    for(int i=0; i<HEIGHT; i++){
        tiles.push_back({});
        for(int j=0; j<WIDTH; j++){
            tile t;
            t.rect={.x=j*TILE_SIZE, .y=i*TILE_SIZE, .w=TILE_SIZE, .h=TILE_SIZE};
            t.state=0, t.speed=0;
            tiles.back().push_back(t);
        }
    }
}

bool get_new_tile(tile& lhs, const tile& rhs, bool stopped){
    if(stopped)
        lhs.state=2;
    else
        lhs.state=rhs.state;
    lhs.c=rhs.c;
    lhs.speed=rhs.speed;
    return 1;
}

void tile_move(std::vector<std::vector<tile>> &tile_map, std::vector<std::vector<tile>> &new_tile_map){
    for(int i=0; i<HEIGHT; i++){
        for(int j=0; j<WIDTH; j++){
            if(tile_map[i][j].state==1){
                int offset=tile_map[i][j].speed;
                while(i+offset>=HEIGHT || tile_map[i+offset][j].state)offset--;//when the i+offset touches an already placed cell it shrinks till the current cell 
                                                                                //cannot touch it
                if(offset<=0)offset=1;//cap the offset

                if(i+offset>=HEIGHT || (tile_map[i+offset][j].state==2 && tile_map[i+offset][j+1==WIDTH?j:j+1].state==2 && tile_map[i+offset][j-1==-1?j:j-1].state==2)){
                    get_new_tile(new_tile_map[i][j], tile_map[i][j], true);
                    continue;
                }

                bool is_placed=0;
                if(!tile_map[i+offset][j].state){
                    is_placed=get_new_tile(new_tile_map[i+offset][j], tile_map[i][j], false);
                }
                else{
                    int dir=rand()%2==0?-1:1;
                    if(0<=j+dir&&j+dir<WIDTH && !tile_map[i+offset][j+dir].state){
                        is_placed=get_new_tile(new_tile_map[i+offset][j+dir], tile_map[i][j], false);
                    }else{
                        dir=dir==-1?1:-1;
                        if(0<=j+dir&&j+dir<WIDTH && !tile_map[i+offset][j+dir].state){
                            is_placed=get_new_tile(new_tile_map[i+offset][j+dir], tile_map[i][j], false);
                        }
                    }
                } 
            }else if(tile_map[i][j].state==2){
                get_new_tile(new_tile_map[i][j], tile_map[i][j], true);
            }
        }
    }
    tile_map=new_tile_map;
    new_tile_map.clear();
}

void tile_draw(SDL_Renderer *rend, std::vector<std::vector<tile>> &tile_map){
    for(int i=0; i<HEIGHT; i++){
        for(int j=0; j<WIDTH; j++){
            if(tile_map[i][j].state>0){
                SDL_SetRenderDrawColor(rend, tile_map[i][j].c.r, tile_map[i][j].c.g, tile_map[i][j].c.b, tile_map[i][j].c.a);
                SDL_RenderFillRect(rend, &tile_map[i][j].rect);
            }else{
                SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
                SDL_RenderFillRect(rend, &tile_map[i][j].rect);
            }
        }
    }
}