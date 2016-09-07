#include <stdio.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#include "SpriteSheet.hpp"

#ifndef Characters_hpp
#define Characters_hpp

enum State {left,right,idling,jumping,falling};

class Object{
protected:
    SDL_Rect dsRect;
    SDL_Renderer* ren;
    SDL_Texture* tex;
    bool dead;
    bool onScreen;
public:
    Object(int xCoord, int yCoord, int width, int height, SDL_Renderer* r,std::string fn);
    Object(){};
    virtual void update();
    virtual void draw();
    virtual bool isEnemy(){return false;};
    void updatePos(int newXCoord, int newYCoord);
    void shiftPos(int dX, int dY);
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    SDL_Rect* getRect();
    SDL_Rect getInteractRect();
    bool isAlive();
    bool isOnScreen();
    virtual SDL_Texture* getTexture();
    void kill(){dead = true;};
    
};

class Tile : public Object{
public:
    Tile(int xCoord, int yCoord, int width, int height, SDL_Renderer* r,std::string fn):Object(xCoord,yCoord,width,height,r,fn){};
};

class Actor : public Object{
protected:
    std::vector<Object*>* tiles;
    State state,stateVertical;
    int xSpeed,ySpeed,xAcc,yAcc;
    int health;
    bool hittingRight,hittingFloor,hittingLeft,hittingCeiling;
public:
    Actor(int xCoord, int yCoord, int width, int height, SDL_Renderer* r, std::string fn,std::vector<Object*>* e):Object(xCoord,yCoord,width,height,r,fn){
        stateVertical = falling;
        state = idling;
        xSpeed = 0;
        ySpeed = 0;
        xAcc = 0;
        yAcc = 0;
        tiles = e;
        health = 10;
    };
    virtual void interactTile(Object* t);
    virtual void hitFloor();
    virtual void hitLeftWall();
    virtual void hitRightWall();
    virtual void hitCeiling();
    virtual void interactActor(Actor* a);
    virtual void update();
    virtual void leftUpdate();
    virtual void rightUpdate();
    virtual void jumpUpdate();
    virtual void fallUpdate();
    virtual void idleUpdate();
    virtual void idleVerticalUpdate();
    virtual void takeHit(int);
    virtual bool isValidLoc(int l, bool forX);
    void setState(State a){state = a;};
    void setStateVertical(State a){stateVertical = a;};
    State getState(){return state;};
    State getVerticalState(){return stateVertical;};
    bool between(int cur, int left, int right);
    virtual bool floating();
    virtual void setTarget(int){};
};

class Player : public Actor{

protected:
    Sheet* walks;
    Sheet* flys;
    Sheet* falls;
    Sheet* idles;
    
    SDL_Texture* goodHealth;
    SDL_Texture* badHealth;
    
    SDL_Rect healthRect;
    SDL_Rect curHealthRect;
    
    std::vector<Sheet*> sheets;
    bool faceRight;
    
    int maxHealth;
    
public:
    Player(int xCoord, int yCoord, int width, int height, SDL_Renderer* r, std::string fn,std::vector<Object*>* e):Actor(xCoord,yCoord,width,height,r,fn,e){
        walks = new Sheet(8,0,30,30,30,r,fn);
        flys = new Sheet(4,0,120,30,30,r,fn);
        falls = new Sheet(2,0,90,30,30,r,fn);
        idles = new Sheet(1,0,0,30,30,r,fn);
        sheets.push_back(walks);
        sheets.push_back(flys);
        sheets.push_back(falls);
        sheets.push_back(idles);
        faceRight = true;
        health = 100;
        maxHealth = 100;
        goodHealth = Static::loadTexture(Static::workingDir+"gh.bmp", ren);
        badHealth = Static::loadTexture(Static::workingDir+"bh.bmp", ren);
    };
    virtual void update();
    virtual void jumpUpdate();
    virtual void hitLeftWall(){};
    virtual void hitRightWall(){};
    virtual bool isValidLoc(int l, bool forX);
    virtual void draw();
    virtual void drawHealth();
    virtual void interactActor(Actor* a);
    virtual void takeHit(int);
    void resetSheets(){
        for(Sheet *a:sheets){
            a->resetSheet();
        }
    };
    virtual SDL_Texture* getTexture(){return nullptr;};
    ~Player(){
        delete flys;
        delete walks;
        delete falls;
        delete idles;
        SDL_DestroyTexture(goodHealth);
        SDL_DestroyTexture(badHealth);
    }
};

class Dunkel : public Player{
protected:
    int prevX,prevY;
    bool justJumped;
public:
    Dunkel(int xCoord, int yCoord, int width, int height, SDL_Renderer* r, std::string fn,std::vector<Object*>* e):Player(xCoord,yCoord,width,height,r,fn,e){
        delete walks;
        delete falls;
        delete flys;
        delete idles;
        sheets.clear();
        walks = new Sheet(5,0,0,60,75,r,fn);
        flys = new Sheet(1,120,0,60,75,r,fn);
        falls = new Sheet(5,0,0,60,75,r,fn);
        idles = new Sheet(2,0,60,60,75,r,fn);
        sheets.push_back(walks);
        sheets.push_back(flys);
        sheets.push_back(falls);
        sheets.push_back(idles);
        state = left;
        justJumped = false;
        health = 10;
        maxHealth = health;
    };
    virtual void hitLeftWall();
    virtual void hitRightWall();
    virtual void update();
    virtual void jumpUpdate();
    virtual void hitFloor();
    virtual bool isValidLoc(int l, bool forX);
    virtual bool isEnemy(){return true;};
    virtual void setTarget(int);
    virtual void leftUpdate();
    virtual void rightUpdate();
    virtual void interactActor(Actor*);
    virtual void drawHealth(){};
};

#endif /* Characters_hpp */
