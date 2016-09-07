#include <stdio.h>
#include <SDL2/SDL.h>
#include <iostream>
#include "StaticMethods.hpp"

#ifndef SpriteSheet_hpp
#define SpriteSheet_hpp

class Sheet{
    
protected:
    int numFrames;
    int currFrame;
    int startX,startY,width,height;
    SDL_Texture* frames;
    SDL_Renderer* ren;
    SDL_Rect captRect;
public:
    Sheet(int numFrames,int startX, int startY, int width,int height,SDL_Renderer* ren, std::string fn);
    void getNextImage(SDL_Rect*,bool);
    void resetSheet(){currFrame = 0;};
    ~Sheet(){
        SDL_DestroyTexture(frames);
    }
};

#endif /* SpriteSheet_hpp */
