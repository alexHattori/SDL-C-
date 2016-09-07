#include <iostream>
#include <SDL2/SDL.h>

#include "StaticMethods.hpp"
#include "Characters.hpp"
#include "GameState.hpp"

int main(int argc, const char * argv[]) {
    
    //Initializes SDL, Window (window), Renderer (ren)
    if(SDL_Init(SDL_INIT_EVERYTHING)<0){
        std::cout << "Could not initialize"<<std::endl;
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("Jorn Dunkel Shmorn Dunkel!",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, Static::SCREEN_WIDTH,Static::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL){
        std::cout << "SDL CreateWindow Error"<<std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(ren == NULL){
        SDL_DestroyWindow(window);
        std::cout << "Renderer Failed" << std::endl;
        return 1;
    }

    GameState* gs = new GameState(window,ren);

    gs->run();
    
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
    delete gs;
    
    return 0;
}
