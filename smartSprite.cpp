#include <cmath>
#include "viewport.h"
#include "smartSprite.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

SmartSprite::SmartSprite(const std::string& name, 
                          Johnny& p):
 Enemy(name),
 io(IOmod::getInstance()), 
 enemy(p)
 { }

void SmartSprite::goUpLeft()  { 
    setVelocityX(-175);
    setVelocityY(-175);
}
void SmartSprite::goDownLeft() {
    setVelocityX(-275);
    setVelocityY(275);
}
void SmartSprite::goUpRight()  { 
    setVelocityX(175);
    setVelocityY(-175);
}
// void SmartSprite::goRight() { velocityX( fabs(velocityX()) ); }
// void SmartSprite::goUp()    { velocityY( -fabs(velocityY()) ); }
// void SmartSprite::goDown()  { velocityY( fabs(velocityY()) ); }

void SmartSprite::draw() const {
  Enemy::draw();
  // int x = 10+ X() - Viewport::getInstance().X();
  // int y = 10+ Y() - Viewport::getInstance().Y();
 
}

void SmartSprite::update(Uint32 ticks) {
  Enemy::update(ticks);
  float x= getX()+getFrame()->getWidth()/2;
  float y= getY()+getFrame()->getHeight()/2;
  float ex= enemy.getX()+enemy.getFrame()->getWidth()/2;
  float ey= enemy.getY()+enemy.getFrame()->getHeight()/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );

    
    if((x-ex) > 0 && (distanceToEnemy < 175)){
      goDownLeft();
    }
    else if((y-ey) > 0 && (distanceToEnemy < 175)){
      goUpLeft();
    }
    else if((y-ey) < 0 && (distanceToEnemy < 175)){
      goDownLeft();
    }


    // if(distanceToEnemy < safeDistance) {
         

    // }
  // }
  // else if  ( currentMode == EVADE ) {
  //   if(distanceToEnemy > safeDistance) currentMode=NORMAL;
  //   else {
  //     if ( x < ex ) goLeft();
  //     if ( x > ex ) goRight();
  //     if ( y < ey ) goUp();
  //     if ( y > ey ) goDown();
  //   }
  }


