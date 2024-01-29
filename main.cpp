#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <SDL2/SDL.h>
using namespace std;

const int TILE_SIZE=3;
const int WINDOW_WIDTH=800,WINDOW_HEIGHT=600;
const int WIDTH=WINDOW_WIDTH/TILE_SIZE, HEIGHT=WINDOW_HEIGHT/TILE_SIZE;
const float RAD=8;

const int TILES_PER_CLICK=5;
const int COLORS_SIZE=7;

struct tile{
    SDL_Rect rect;
    SDL_Color c;
    int state, speed;
};

void fill_tiles(vector<vector<tile>> &tiles, bool first);
bool is_inside_circle(int mx, int my, int x, int y);
bool get_new_tile(tile& lhs, const tile& rhs);
SDL_Color blendColors(SDL_Color color1, SDL_Color color2, float t);
vector<SDL_Color> get_gradients();
float delta_time(Uint64 &last_tick_t);

Uint64 last_tick_t=0;
int color_ind=0;
vector<vector<tile>> tile_map;
 SDL_Color colors[COLORS_SIZE] = {
    {255, 0, 0, 255},     // Red
    {255, 127, 0, 255},   // Orange
    {255, 255, 0, 255},   // Yellow
    {0, 255, 0, 255},     // Green
    {0, 0, 255, 255},     // Blue
    {75, 0, 130, 255},    // Indigo
    {148, 0, 211, 255}    // Violet
};

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

    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    fill_tiles(tile_map, 1);

    auto gradient_colors=get_gradients();
    vector<vector<tile>> new_tile_map;
    int mx, my, color_cnt=0;
    bool should_close=0;

    while(!should_close){
        fill_tiles(new_tile_map, 0);

        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT)should_close=1;

            int mx, my;
            Uint32 m_state=SDL_GetMouseState(&mx, &my);

            if(m_state&SDL_BUTTON(SDL_BUTTON_LEFT)){                
                mx/=TILE_SIZE,my/=TILE_SIZE;
                for(int i=my-TILES_PER_CLICK; i<=my+TILES_PER_CLICK; i++){
                    for(int j=mx-TILES_PER_CLICK; j<=mx+TILES_PER_CLICK; j++){
                        if(0<=j&&j<WIDTH&&0<=i&&i<HEIGHT){
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

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);

        auto delta=delta_time(last_tick_t);
        for(int i=0; i<HEIGHT; i++){
            for(int j=0; j<WIDTH; j++){
                if(tile_map[i][j].state){
                    int offset=tile_map[i][j].speed;
                    if(i+offset>=HEIGHT || (tile_map[i+offset][j].state && tile_map[i+offset][j+1].state && tile_map[i+offset][j-1].state)){
                        get_new_tile(new_tile_map[i][j], tile_map[i][j]);
                        continue;
                    }
                    bool is_placed=0;
                    if(!tile_map[i+offset][j].state){
                        is_placed=get_new_tile(new_tile_map[i+offset][j], tile_map[i][j]);
                    }
                    else{
                        int dir=rand()%2==0?-1:1;
                        if(0<=j+dir && j+dir<WIDTH && !tile_map[i+offset][j+dir].state){
                            is_placed=get_new_tile(new_tile_map[i+offset][j+dir], tile_map[i][j]);
                        }else{
                            dir=dir==-1?1:-1;
                            if(0<=j+dir && j+dir<WIDTH && !tile_map[i+offset][j+dir].state){
                                is_placed=get_new_tile(new_tile_map[i+offset][j+dir], tile_map[i][j]);is_placed=1;
                            }
                        }
                    } 

                    if(!is_placed){
                        get_new_tile(new_tile_map[i][j], tile_map[i][j]);
                    }
                }
            }
        }

        tile_map=new_tile_map;
        new_tile_map.clear();

        for(int i=0; i<HEIGHT; i++){
            for(int j=0; j<WIDTH; j++){
                if(!tile_map[i][j].state){
                    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
                    SDL_RenderFillRect(rend, &tile_map[i][j].rect);
                }else{
                    SDL_SetRenderDrawColor(rend, tile_map[i][j].c.r, tile_map[i][j].c.g, tile_map[i][j].c.b, tile_map[i][j].c.a);
                    SDL_RenderFillRect(rend, &tile_map[i][j].rect);
                }
            }
        }
        SDL_RenderPresent(rend);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}

void fill_tiles(vector<vector<tile>> &tiles, bool first){
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
SDL_Color blendColors(SDL_Color color1, SDL_Color color2, float t) {
    SDL_Color result;
    result.r = (1 - t) * color1.r + t * color2.r;
    result.g = (1 - t) * color1.g + t * color2.g;
    result.b = (1 - t) * color1.b + t * color2.b;
    result.a = 255; 
    return result;
}

vector<SDL_Color> get_gradients(){
    int colorsPerSection = 10; 
    int totalColors = (COLORS_SIZE - 1) * colorsPerSection + 1;
    vector<SDL_Color> gradientRainbow(totalColors);

    for (int i = 0; i < COLORS_SIZE - 1; ++i) {
        for (int j = 0; j < colorsPerSection; ++j) {
            float t = (float)j / (colorsPerSection - 1);
            gradientRainbow[i * colorsPerSection + j] = blendColors(colors[i], colors[i + 1], t);
        }
    }
    gradientRainbow[totalColors - 1] = colors[COLORS_SIZE - 1];

    return gradientRainbow;
}

bool is_inside_circle(int mx, int my, int x, int y){
    float eq=pow((x - mx), 2) + pow((y-my),2);

    return eq==RAD||sqrt(eq)<=RAD;
}

bool get_new_tile(tile& lhs, const tile& rhs){
    lhs.state=rhs.state;
    lhs.c=rhs.c;
    lhs.speed=rhs.speed;
    return 1;
}

float delta_time(Uint64 &last_tick_t){
    Uint64 tick_t=SDL_GetPerformanceCounter();
    float delta=(float)((tick_t-last_tick_t)*1000 / (float) SDL_GetPerformanceCounter());
    last_tick_t=tick_t;

    return delta;
}