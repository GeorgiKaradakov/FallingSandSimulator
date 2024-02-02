#pragma once
#include <SDL2/SDL.h>
#include <vector>

#include "global_vars.h"
#include "tile.h"

SDL_Color colors[COLORS_SIZE] = {
    {255, 0, 0, 255},     // Red
    {255, 127, 0, 255},   // Orange
    {255, 255, 0, 255},   // Yellow
    {0, 255, 0, 255},     // Green
    {0, 0, 255, 255},     // Blue
    {75, 0, 130, 255},    // Indigo
    {148, 0, 211, 255}    // Violet
};

bool is_inside_circle(int mx, int my, int x, int y){
    float eq=(float)pow((x-mx),2)+(float)pow((y-my),2);

    return sqrt(eq)<=RAD;
}

void proccess_events(std::vector<std::vector<tile>> &tile_map, std::vector<SDL_Color> &gradient_colors, SDL_Event &e, bool &should_close, bool &space_pressed){
    while(SDL_PollEvent(&e)){
        if(e.type==SDL_QUIT)should_close=1;
        else if(e.type==SDL_KEYDOWN){
            if(e.key.keysym.sym==SDLK_SPACE)
                space_pressed=1;
            else if(e.key.keysym.sym==SDLK_ESCAPE)
                should_close=1;
        }else if(e.type==SDL_KEYUP){
            if(e.key.keysym.sym==SDLK_SPACE)
                space_pressed=0;
        }

        int mx, my;
        Uint32 m_state=SDL_GetMouseState(&mx, &my);

        if(m_state&SDL_BUTTON(SDL_BUTTON_LEFT)){                
            if(0<=mx&&0<WINDOW_WIDTH && 0<=my&&my<WINDOW_HEIGHT){
                mx/=TILE_SIZE,my/=TILE_SIZE;

                int starti=my-TILES_PER_CLICK, startj=mx-TILES_PER_CLICK, endi=my+TILES_PER_CLICK, endj=mx+TILES_PER_CLICK;

                for(int i=starti; i<=endi; i++){
                    for(int j=startj; j<=endj; j++){
                        if(0<=j&&j<WIDTH&&0<i&&i<HEIGHT){
                            if(is_inside_circle(mx, my, j, i)&&!tile_map[i][j].state){
                                tile_map[i][j].state=1;
                                tile_map[i][j].c=gradient_colors[color_ind%gradient_colors.size()];
                                tile_map[i][j].speed=rand()%5+4;
                            }
                        }
                    }
                }
                if(!(color_cnt++%10))color_ind++;
            }
        }
    }
}

SDL_Color blendColors(SDL_Color color1, SDL_Color color2, float t) {
    SDL_Color result;
    result.r=(1-t)*color1.r+t*color2.r;
    result.g=(1-t)*color1.g+t*color2.g;
    result.b=(1-t)*color1.b+t*color2.b;
    result.a=255; 
    return result;
}

std::vector<SDL_Color> get_gradients(){
    int colorsPerSection=10; 
    int totalColors=(COLORS_SIZE-1)*colorsPerSection+1;
    std::vector<SDL_Color> gradientRainbow(totalColors);

    for (int i=0; i<COLORS_SIZE-1; ++i) {
        for (int j=0; j<colorsPerSection; ++j) {
            float t=(float)j/(colorsPerSection-1);
            gradientRainbow[i*colorsPerSection+j]=blendColors(colors[i],colors[i+1],t);
        }
    }
    gradientRainbow[totalColors-1]=colors[COLORS_SIZE-1];

    return gradientRainbow;
}


void set_color(SDL_Renderer *rend, const SDL_Color &color){
    SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);
}