#include <SDL2/SDL.h>
#include <iostream>

#ifndef StaticMethods_hpp
#define StaticMethods_hpp

class Static{
public:
    static int SCREEN_WIDTH;
    static int SCREEN_HEIGHT;
    
    static int TILE_WIDTH;
    static int TILE_HEIGHT;
    static int ACTOR_WIDTH;
    static int ACTOR_HEIGHT;
    
    static int X_ACCELERATION;
    static int Y_ACCELERATION;
    static int MAX_X_VELOCITY;
    static int MAX_Y_VELOCITY;
    
    static int MAX_DUNK_X_VELOCITY;
    
    static int PLAYER_DAMAGE;
    static int DUNKEL_DAMAGE;
    
    static std::string workingDir;
    static SDL_Texture* loadTexture(const std::string &path, SDL_Renderer *ren,bool removeBackground = false);
};


#endif /* StaticMethods_hpp */
