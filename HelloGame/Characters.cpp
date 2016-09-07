#include "StaticMethods.hpp"

#include "Characters.hpp"

Object::Object(int xCoord, int yCoord, int width, int height, SDL_Renderer* r,std::string fn){
    dsRect.x = xCoord;
    dsRect.y = yCoord;
    dsRect.w = width;
    dsRect.h = height;
    ren = r;
    tex = Static::loadTexture(fn, ren, true);
    dead = false;
    onScreen = false;
}
void Object::update(){
    if(getX()+getWidth()<-2*Static::TILE_WIDTH){
        dead = true;
        return;
    }
    else if(getY() > Static::SCREEN_HEIGHT){
        dead = true;
    }
    if(getX()>Static::SCREEN_WIDTH){
        onScreen = false;
    }
    else if(getY()+getHeight()<0){
        onScreen = false;
    }
    else if(!dead){
        onScreen = true;
    }
}
void Object::draw(){
    SDL_RenderCopy(ren, tex, NULL, &dsRect);
}
void Object::updatePos(int newXCoord, int newYCoord){
    dsRect.x = newXCoord;
    dsRect.y = newYCoord;
}
void Object::shiftPos(int dX, int dY){
    dsRect.x+=dX;
    dsRect.y+=dY;
}
int Object::getX(){
    return dsRect.x;
}
int Object::getY(){
    return dsRect.y;
}
int Object::getWidth(){
    return dsRect.w;
}
int Object::getHeight(){
    return dsRect.h;
}
SDL_Rect* Object::getRect(){
    return &dsRect;
}
SDL_Rect Object::getInteractRect(){
    SDL_Rect newRect = dsRect;
    newRect.x--;
    newRect.y--;
    newRect.w+=2;
    newRect.h+=2;
    return newRect;
}
bool Object::isAlive(){
    return !dead;
}
bool Object::isOnScreen(){
    return onScreen;
}
SDL_Texture* Object::getTexture(){
    return tex;
}

bool Actor::isValidLoc(int l, bool forX){
    SDL_Rect tempRect;
    if(forX){
        tempRect.x = l;
        tempRect.y = getRect()->y;
    }
    else{
        tempRect.x = getRect()->x;
        tempRect.y = l;
    }
    tempRect.w = getRect()->w;
    tempRect.h = getRect()->h;
    for(Object* a:*tiles){
        SDL_Rect n1= a->getInteractRect();
        SDL_Rect* n = &n1;
        if(SDL_HasIntersection(&tempRect,n)==SDL_TRUE){
            return false;
        }
    }
    return true;
}
void Actor::update(){
    Object::update();
    if(health<=0){
        dead = true;
    }
    int tempXLoc = getRect()->x,tempYLoc = getRect()->y;
    if(state == left){
        leftUpdate();
    }
    else if(state==right){
        rightUpdate();
    }
    if(stateVertical == jumping){
        jumpUpdate();
    }
    else if(stateVertical == falling){
        fallUpdate();
    }
    if(state == idling){
        idleUpdate();
    }
    if(stateVertical==idling){
        idleVerticalUpdate();
    }
    tempXLoc = getRect()->x+xSpeed;
    tempYLoc = getRect()->y+ySpeed;
    while(!isValidLoc(tempXLoc,true)&&tempXLoc!=getRect()->x){
        tempXLoc -= xSpeed/abs(xSpeed);
    }
    while(!isValidLoc(tempYLoc,false)&&tempYLoc!=getRect()->y){
        tempYLoc -= ySpeed/abs(ySpeed);
    }
    updatePos(tempXLoc, tempYLoc);
}
void Actor::leftUpdate(){
    if(xSpeed>-Static::MAX_X_VELOCITY){
        xSpeed-= Static::X_ACCELERATION;
    }
}
void Actor::rightUpdate(){
    if(xSpeed<Static::MAX_X_VELOCITY){
        xSpeed+=Static::X_ACCELERATION;
    }
}
void Actor::jumpUpdate(){
    if(-ySpeed<Static::MAX_Y_VELOCITY){
        ySpeed-=Static::Y_ACCELERATION;
    }
    else{
        stateVertical = falling;
    }
}
void Actor::fallUpdate(){
    if(ySpeed<Static::MAX_Y_VELOCITY){
        ySpeed+=Static::Y_ACCELERATION;
    }
}
void Actor::idleUpdate(){
    if(abs(xSpeed)>0){
        xSpeed-=Static::X_ACCELERATION*xSpeed/abs(xSpeed);
    }
}
void Actor::idleVerticalUpdate(){
    if(floating()){
        stateVertical = falling;
    }
}
void Actor::interactTile(Object* t){
    SDL_Rect myRect = getInteractRect();
    SDL_Rect otherRect = (*t).getInteractRect();
    
    //Case Actor above tile
    if(stateVertical==falling){
        if(myRect.y+myRect.w>=otherRect.y){
            hitFloor();
            hittingFloor = true;
        }
        else{
            hittingFloor = false;
        }
    }
    //Case Actor below tile
    else if(stateVertical==jumping){
        if(myRect.y<=otherRect.y+otherRect.w&&myRect.y+myRect.h>otherRect.y+otherRect.h){
            int avgLoc = (int)((myRect.x*2+myRect.w)/2);
            if(between(avgLoc,otherRect.x,otherRect.x+otherRect.w)){
                hitCeiling();
                hittingCeiling = true;
            }
            else{
                hittingCeiling = false;
            }
        }
    }
    if(state == right){
        if(myRect.x+myRect.w>=otherRect.x&&myRect.x<otherRect.x){
            int avgLoc = (int)((myRect.y*2+myRect.h)/2);
            if(between(avgLoc,otherRect.y,otherRect.y+otherRect.h)){
                hitLeftWall();
                hittingLeft = true;
            }
            else{
                hittingLeft = false;
            }
        }
    }
    else if (state == left){
        if(myRect.x<=otherRect.x+otherRect.w&&myRect.x+myRect.w>otherRect.x+otherRect.w){
            int avgLoc = (int)((myRect.y*2+myRect.h)/2);
            if(between(avgLoc,otherRect.y,otherRect.y+otherRect.h)){
                hitRightWall();
                hittingRight = true;
            }
            else{
                hittingRight = false;
            }

        }
    }
}
void Actor::interactActor(Actor* a){
    
}
void Actor::hitFloor(){
    stateVertical = idling;
}
void Actor::hitCeiling(){
    stateVertical = falling;
}
void Actor::hitLeftWall(){
    state = left;
}
void Actor::hitRightWall(){
    state = right;
}
bool Actor::between(int cur,int left, int right){
    return (cur<=right && cur>=left);
}
bool Actor::floating(){
    for(Object* a : *tiles){
        SDL_Rect n1= a->getInteractRect();
        SDL_Rect* n = &n1;
        SDL_Rect rect = getInteractRect();
        SDL_Rect* myRect = &rect;
        if(SDL_HasIntersection(myRect,n)==SDL_TRUE){
            return false;
        }

    }
    return !(stateVertical==jumping);
}
void Actor::takeHit(int damage){
    dead = true;
}

void Player::update(){
    Actor::update();
    if(state == right){
        faceRight = true;
    }
    else if(state == left){
        faceRight = false;
    }
    
}
void Player::jumpUpdate(){
    if(-ySpeed<Static::MAX_Y_VELOCITY){
        ySpeed-=Static::Y_ACCELERATION;
    }
}
bool Player::isValidLoc(int l, bool forX){
    if(l<=0){
        return false;
    }
    if(l+getWidth()>=Static::SCREEN_WIDTH){
        return false;
    }
    return Actor::isValidLoc(l, forX);
}
void Player::draw(){
    if(faceRight){
        if(stateVertical == jumping){
            flys->getNextImage(&dsRect,false);
        }
        else if(stateVertical == falling){
            falls->getNextImage(&dsRect,false);
        }
        else if(state!=idling){
            walks->getNextImage(&dsRect,false);
        }
        else{
            idles->getNextImage(&dsRect, false);
        }
    }
    else{
        if(stateVertical == jumping){
            flys->getNextImage(&dsRect,true);
        }
        else if(stateVertical == falling){
            falls->getNextImage(&dsRect,true);
        }
        else if(state!=idling){
            walks->getNextImage(&dsRect,true);
        }
        else{
            idles->getNextImage(&dsRect, true);
        }

    }
    drawHealth();
    
}
void Player::drawHealth(){
    healthRect.x = dsRect.x;
    healthRect.y = dsRect.y-12;
    healthRect.w = dsRect.w;
    healthRect.h = 10;
    
    curHealthRect.x = dsRect.x;
    curHealthRect.y = dsRect.y-12;
    curHealthRect.w = (int)(((double)health/maxHealth)*dsRect.w);
    curHealthRect.h = 10;
    
    SDL_RenderCopy(ren, badHealth, NULL, &healthRect);
    SDL_RenderCopy(ren, goodHealth, NULL, &curHealthRect);
}
void Player::interactActor(Actor* a){
    if(getY()+getHeight()>a->getY()&& getY()+getHeight()<a->getY()+a->getHeight()){
        a->takeHit(Static::PLAYER_DAMAGE);
    }
}
void Player::takeHit(int damage){
    health-=damage;
}

void Dunkel::hitLeftWall(){
    if(!justJumped){
        stateVertical = jumping;
        justJumped = true;
    }
}
void Dunkel::hitRightWall(){
    if(!justJumped){
        stateVertical = jumping;
        justJumped = true;
    }
}
void Dunkel::update(){
    Player::update();
    if(prevY == getY()&&stateVertical == falling&&!justJumped){
        stateVertical = jumping;
        justJumped = true;
    }
    else{
        prevX = getX();
        prevY = getY();
    }
    if(floating()&&getY()>Static::SCREEN_HEIGHT-Static::ACTOR_HEIGHT){
        stateVertical = jumping;
        justJumped = true;
    }
}
void Dunkel::jumpUpdate(){
    Actor::jumpUpdate();
}
void Dunkel::hitFloor(){
    Actor::hitFloor();
    justJumped = false;
}
bool Dunkel::isValidLoc(int l, bool forX){
    if(l<=0){
        if(forX){
            state = right;
        }
        return false;
    }
    if(forX){
        if(l+getWidth()>=Static::SCREEN_WIDTH){
            state = left;
            return false;
        }
    }
    return Actor::isValidLoc(l, forX);
}
void Dunkel::setTarget(int playX){
    if(playX<getX()){
        state = left;
    }
    else{
        state = right;
    }
}
void Dunkel::leftUpdate(){
    if(xSpeed>-Static::MAX_DUNK_X_VELOCITY){
        xSpeed-= Static::X_ACCELERATION;
    }
}
void Dunkel::rightUpdate(){
    if(xSpeed<Static::MAX_DUNK_X_VELOCITY){
        xSpeed+=Static::X_ACCELERATION;
    }
}
void Dunkel::interactActor(Actor* a){
    if(!(a->getX()<getX())&&!a->isEnemy())
        a->takeHit(Static::DUNKEL_DAMAGE);
}

