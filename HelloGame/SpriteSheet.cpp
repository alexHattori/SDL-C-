//
//  SpriteSheet.cpp
//  HelloGame
//
//  Created by Alex Hattori on 10/23/15.
//  Copyright © 2015 Alex Hattori. All rights reserved.
//

#include "SpriteSheet.hpp"

Sheet::Sheet(int nf,int x, int y,int targetWidth,int targetHeight,SDL_Renderer* r, std::string fn){
    numFrames = nf;
    currFrame = 0;
    startX = x;
    startY = y;
    width = targetWidth;
    height = targetHeight;
    ren = r;
    frames = Static::loadTexture(fn, r,true);
}
void Sheet::getNextImage(SDL_Rect* dsRect,bool reversed){
    captRect.y = startY;
    captRect.x = (currFrame*width)+startX;
    captRect.w = width;
    captRect.h = height;
    if(!reversed)
        SDL_RenderCopy(ren, frames, &captRect, dsRect);
    else
        SDL_RenderCopyEx(ren, frames, &captRect, dsRect, 0, NULL, SDL_FLIP_HORIZONTAL);
    if(currFrame<numFrames-1){
        currFrame++;
    }
    else{
        currFrame = 0;
    }
    
}