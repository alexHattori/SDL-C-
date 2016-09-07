#include <stdio.h>
#include <SDL2/SDL.h>
#include <vector>


#ifndef MenuState_hpp
#define MenuState_hpp

class MenuState{
protected:
    SDL_Window* window;
    SDL_Renderer* ren;
    
public:
    MenuState(SDL_Window* w, SDL_Renderer* r){window = w; ren = r;};
    void run();
    };

#endif /* MenuState_hpp */
