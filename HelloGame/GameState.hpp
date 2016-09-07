#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <stdio.h>
#include <time.h>

#ifndef GameState_hpp
#define GameState_hpp

class GameState{
protected:
    SDL_Window* window;
    SDL_Renderer* ren;
public:
    GameState(SDL_Window* w, SDL_Renderer* r){window = w; ren = r;};
    void run();
    int spaceToLoc(int x){
        x-=1;
        return Static::SCREEN_HEIGHT - x*Static::TILE_HEIGHT;
    };
    void generateMap(std::vector<Object*>* e, std::vector<Actor*>*actors,SDL_Renderer* ren, int start, int end){
        
        bool prevHole = false;
        int curLoc = start;
        
        const int numVerticalSpaces = (int)(Static::SCREEN_HEIGHT/Static::TILE_HEIGHT);
        const int absMinSpace = 1;
        const int absMaxSpace = numVerticalSpaces-3;
        
        int lastVLoc = 2;
        int minVLoc = 0;
        int maxVLoc = 2;
        int curVLoc = 1;
        srand((uint)(time(0)));
        while(curLoc < end-Static::TILE_WIDTH){
            if(prevHole || rand()%5!=0){
                prevHole = false;
                minVLoc = lastVLoc-1;
                maxVLoc = lastVLoc+1;
                curVLoc = rand()%maxVLoc+minVLoc;
                while(curVLoc>maxVLoc){
                    curVLoc--;
                }
                while(curVLoc<minVLoc){
                    curVLoc++;
                }
                if(curVLoc > 2){
                    for(int i = 0; i < curVLoc; i++){
                        Tile* newTile = new Tile(curLoc,spaceToLoc(i),Static::TILE_WIDTH,Static::TILE_HEIGHT,ren,Static::workingDir+"tile.bmp");
                        e->push_back(newTile);
                        
                    }
                }
            }
            else{
                prevHole = true;
                curVLoc = 1;
                lastVLoc = curVLoc;
                curLoc+=Static::TILE_WIDTH;
                continue;
            }
            
            while(curVLoc<=absMinSpace){
                curVLoc++;
            }
            while(curVLoc>=absMaxSpace){
                curVLoc--;
            }
            lastVLoc = curVLoc;
            Tile* newTile = new Tile(curLoc,spaceToLoc(curVLoc),Static::TILE_WIDTH,Static::TILE_HEIGHT,ren,Static::workingDir+"tile.bmp");
            e->push_back(newTile);
            curLoc+=Static::TILE_WIDTH;
        }
        
        int numDunks = 5;
        while(numDunks>0){
            int xLoc = rand()%end-Static::ACTOR_WIDTH+start;
            Dunkel* test = new Dunkel(xLoc,0,Static::ACTOR_WIDTH,Static::ACTOR_HEIGHT,ren,Static::workingDir+"ds.bmp",e);
            
            actors->push_back(test);
            
            numDunks--;
        }
        
    };
    void shiftMap(std::vector<Object*>* e,std::vector<Actor*>* actors, int* ls, int dx, int dy){
        (*ls)+=dx;
        for(Object* a:*e){
            (*a).shiftPos(dx, dy);
        }
        for(Actor* actor:*actors){
            (*actor).shiftPos(dx,dy);
        }
    };
    
};
#endif /* GameState_hpp */
