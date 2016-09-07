#include "StaticMethods.hpp"

SDL_Texture* Static::loadTexture(const std::string &path, SDL_Renderer *ren,bool removeBackground){
    SDL_Texture *texture = NULL;
    SDL_Surface *loadedImage = SDL_LoadBMP(path.c_str());
    
    if(loadedImage!=NULL){
        if(removeBackground)
            SDL_SetColorKey(loadedImage, SDL_TRUE, SDL_MapRGB(loadedImage->format, 255, 255, 255));
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
    }
    else{
        std::cout << "Failed to load image!" <<std::endl;
    }
    return texture;
}
int Static::SCREEN_WIDTH = 650;
int Static::SCREEN_HEIGHT = 500;

int Static::TILE_WIDTH = 50;
int Static::TILE_HEIGHT = 50;
int Static::ACTOR_WIDTH = 45;
int Static::ACTOR_HEIGHT = 45;

int Static::X_ACCELERATION = 1;
int Static::Y_ACCELERATION = 1;

int Static::MAX_X_VELOCITY = 10;
int Static::MAX_Y_VELOCITY = 10;

int Static::MAX_DUNK_X_VELOCITY = 5;

int Static::PLAYER_DAMAGE = 10;
int Static::DUNKEL_DAMAGE = 1;

std::string Static::workingDir = "/Users/Alex/Documents/XCodeWorkspace/HelloGame/";