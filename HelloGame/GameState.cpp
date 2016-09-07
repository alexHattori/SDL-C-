#include "StaticMethods.hpp"
#include "Characters.hpp"

#include "GameState.hpp"

void GameState::run(){
    
    //Needed variables
    bool running = true;
    SDL_Event e;
    std::vector<Object*> tiles;
    std::vector<Actor*> actors;
    int lastGenerationSpot;
    
    //LoadMainImages
    
    //Background**********
    std::string backgroundImagePath = Static::workingDir + "bg.bmp";
    SDL_Texture* backgroundTexture = Static::loadTexture(backgroundImagePath, ren);
    
    //Create Starting Tiles ***************
    Tile* a = new Tile(0,Static::SCREEN_HEIGHT-Static::TILE_HEIGHT,Static::TILE_WIDTH,Static::TILE_HEIGHT,ren,Static::workingDir + "tile.bmp");
    Tile* b = new Tile(Static::TILE_WIDTH,Static::SCREEN_HEIGHT-Static::TILE_HEIGHT,Static::TILE_WIDTH,Static::TILE_HEIGHT,ren,Static::workingDir + "tile.bmp");
    tiles.push_back(a);
    tiles.push_back(b);
    
    //Create Player
    Player* player = new Player(0,0,Static::ACTOR_WIDTH,Static::ACTOR_HEIGHT,ren,Static::workingDir+"ps.bmp",&tiles);
    actors.push_back(player);
    
    //For Testing
    Dunkel* test = new Dunkel(300,0,Static::ACTOR_WIDTH,Static::ACTOR_HEIGHT,ren,Static::workingDir+"ds.bmp",&tiles);
    actors.push_back(test);
    //Create Next Map Segment************
    generateMap(&tiles, &actors,ren, 2*Static::TILE_WIDTH, Static::SCREEN_WIDTH);
    lastGenerationSpot = Static::SCREEN_WIDTH;
    
    //Game Loop
    while(running){
        //Delete Everything and Render Background
        SDL_RenderClear(ren);
            SDL_RenderCopy(ren, backgroundTexture, NULL, NULL);
        
        //If last generated tile is on screen, generate more
        if(lastGenerationSpot<Static::SCREEN_WIDTH){
            generateMap(&tiles,&actors, ren, lastGenerationSpot, lastGenerationSpot+Static::SCREEN_WIDTH);
            lastGenerationSpot+=Static::SCREEN_WIDTH;
        }
        //Update and display tiles
        for(int i = 0; i < tiles.size();i++){
            Object* tile = tiles[i];
            if(!(tile->isAlive())){
                SDL_DestroyTexture(tiles[i]->getTexture());
                delete tiles[i];
                tiles.erase(tiles.begin()+i);
                continue;
            }
            else{
                tile->update();
                if(tile->isOnScreen()){
                    tile->draw();
                }
            }
        }
        //Update and display actors in here:
        if(!player->isAlive()){
            running = false;
        }
        for(int i = 0; i < actors.size();i++){
            Actor* a = actors[i];
            if(!(a->isAlive())){
                
                SDL_DestroyTexture(actors[i]->getTexture());
                delete (Dunkel*)actors[i];
                actors.erase(actors.begin()+i);
                continue;
            }
            else{
                if(a->isEnemy()){
                    a->setTarget(player->getX());
                }
                bool interacted = false;
                for(Object* tile:tiles){
                    SDL_Rect myRect = a->getInteractRect();
                    SDL_Rect theirRect = tile->getInteractRect();
                    if(SDL_HasIntersection(&myRect, &theirRect)==SDL_TRUE){
                        a->interactTile(tile);
                        interacted = true;
                    }
                }
                for(Actor* b: actors){
                    if(b!=a){
                        if(SDL_HasIntersection(b->getRect(), a->getRect())==SDL_TRUE){
                            a->interactActor(b);
                        }
                    }
                }
                a->update();
                if(a->isOnScreen()){
                    a->draw();
                }
            }
        }
        SDL_RenderPresent(ren);
        if(player->getX()>Static::SCREEN_WIDTH/2){
            shiftMap(&tiles, &actors, &lastGenerationSpot, -(player->getX())+Static::SCREEN_WIDTH/2, 0);
        }
        while(SDL_PollEvent( &e ) != 0){
            switch(e.type){
                case SDL_QUIT:
                    running = false;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case SDLK_LEFT:
                            player->setState(left);
                            break;
                        case SDLK_RIGHT:
                            player->setState(right);
                            break;
                        case SDLK_UP:
                            if(player->getState()!=falling){
                                player->setStateVertical(jumping);
                            }
                            break;
                    }
                    break;
                default:
                    player->setState(idling);
                    player->setStateVertical(falling);
                    break;
            }
        }
        SDL_Delay(20);
    }
    //Cleanup
    SDL_DestroyTexture(backgroundTexture);
    for(Object* a: tiles){
        SDL_DestroyTexture(a->getTexture());
        delete a;
    }
    for(Actor* a:actors){
        SDL_DestroyTexture(a->getTexture());
        delete a;
    }
}

