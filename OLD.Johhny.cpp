#include "Johnny.h"
#include "gamedata.h"
#include "renderContext.h"


Johnny::Johnny(const std::string& name) :
   MultiSprite(name),
   clock(Clock::getInstance()),
   run( RenderContext::getInstance()->getFrames(name) ),
   runLeft( RenderContext::getInstance()->getFrames(name + "Left")),
   jump( RenderContext::getInstance()->getFrames("jump")),
   jumpLeft( RenderContext::getInstance()->getFrames("jumpLeft")),
   
   numberOfJump( Gamedata::getInstance().getXmlInt("jump/frames") ),
   jumpInterval( Gamedata::getInstance().getXmlInt("jump/frameInterval")),
   //frameWidth(run[0]->getWidth()),
  // frameHeight(run[0]->getHeight()),
   jumpWidth(jump[0]->getWidth()),
   jumpHeight(jump[0]->getHeight()),
   right(true),
   jumpbool(false),
   jumpR(false),
   jumpL(false),
   jumpStart(0),
   jumpEnd(0),
   bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
   bullets( bulletName ),
   minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") )
   
   {
    frames = &run;
    numberOfFrames = Gamedata::getInstance().getXmlInt(name+"/frames");
    frameInterval = Gamedata::getInstance().getXmlInt(name+"/frameInterval");
    frameWidth = run[0]->getWidth();
    frameHeight = run[0]->getHeight();

   }


Johnny::Johnny(const Johnny& s) :
   MultiSprite(s), 
   clock(s.clock),
   run(s.run),
   runLeft(s.runLeft),
   jump(s.jump),
   jumpLeft(s.jumpLeft),
   numberOfJump(s.numberOfJump),
   jumpInterval(s.jumpInterval),
   jumpWidth(s.jumpWidth),
   jumpHeight(s.jumpHeight),
   right(s.right),
   jumpbool(s.jumpbool),
   jumpR(s.jumpR),
   jumpL(s.jumpL),
   jumpStart(s.jumpStart),
   jumpEnd(s.jumpEnd),
   bulletName(s.bulletName),
   bullets(s.bullets),
   minSpeed(s.minSpeed)
   {}

void Johnny::shoot() { 

  float x = getX()+getFrame()->getWidth();
  float y = getY()+getFrame()->getHeight()/2;
  // I'm not adding minSpeed to y velocity:
  bullets.shoot( Vector2f(x, y), 
    Vector2f(minSpeed+getVelocityX(), 0)
  );

}  

bool Johnny::collidedWith(const Drawable* obj) const {
  return bullets.collidedWith( obj );
}

void Johnny::draw() const { 
  frames->at(currentFrame)->draw(getX(), getY());
  bullets.draw();
        //std::cout << "in Johnny.draw()" << std::endl;

}
/*void Johnny::draw() const { 
  frames->at(currentFrame)->draw(getX(), getY());
}*/

void Johnny::setRight() {
    frames = &run;
    numberOfFrames = Gamedata::getInstance().getXmlInt("run/frames");
    frameInterval = Gamedata::getInstance().getXmlInt("run/frameInterval");
    frameWidth = run[0]->getWidth();
    frameHeight = run[0]->getHeight();
}
void Johnny::setLeft() {
    frames = &runLeft;
    numberOfFrames = Gamedata::getInstance().getXmlInt("run/frames");
    frameInterval = Gamedata::getInstance().getXmlInt("run/frameInterval");
    frameWidth = run[0]->getWidth();
    frameHeight = run[0]->getHeight();
}
void Johnny::jumpUp(){
  numberOfFrames = numberOfJump;
     frameInterval = jumpInterval;
     frameWidth = jumpWidth;
     frameHeight = jumpHeight;
    
        frames = &jump;
        setVelocityX( fabs(0.0) );
        setVelocityY( -fabs( 10.0 ) ); 
        jumpR = true;
        jumpL = false;
        jumpStart = clock.getSeconds();
    
}
void Johnny::jumpLR(){
  // if (jumpbool == false){
       std::cout << "Johnny::jumpLR() top" << std::endl;

     numberOfFrames = numberOfJump;
     frameInterval = jumpInterval;
     frameWidth = jumpWidth;
     frameHeight = jumpHeight;
     if(jumpR){
        jumpL = false;
        frames = &jump;
        setVelocityX( fabs( 375.0 ) );
        setVelocityY( -fabs( 25.0 ) ); 
     }
     if(!jumpR){
        jumpL = true;
        frames = &jumpLeft;
        setVelocityX( -fabs( 375.0 ) );
        setVelocityY( -fabs( 25.0 ) );
     }
     jumpStart = clock.getSeconds();
     std::cout << "Johnny::jumpLR() bottom" << std::endl;
    // jumpbool = true;
// }
}
void Johnny::goRight() {
  setRight();
  setVelocityX( fabs( 375.0 ) );
  setVelocityY(0);
  right = true;
  jumpR = false;
  jumpL = false;
}

void Johnny::goLeft() {
  setLeft();
  setVelocityX( -fabs( 375.0 ) );
  setVelocityY(0);
  right = false;
  jumpR = false;
  jumpL = false;
}

void Johnny::stop() {
  
  right = true;
  jumpR = false;
  jumpL = false;
  setRight();
  setVelocityX(0);
  setVelocityY(0);
}

void Johnny::update(Uint32 ticks) { 
  if(jumpR || jumpL){
    jumpEnd = clock.getSeconds();
    if (jumpEnd - jumpStart > 1 ) { 
      setRight();
      setVelocityX( fabs( 375.0 ) );
      setVelocityY(0);
      right = true;
      jumpR = false;
      jumpL = false;
    }

 }
  advanceFrame(ticks);
  bullets.update(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-frameHeight) { 
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
    frames = &run;
  }
  if ( getX() > worldWidth-frameWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
    frames = &runLeft;
  }  

}
/* if(jumpR || jumpL){
    jumpEnd = clock.getSeconds();
    if (jumpEnd - jumpStart > 1 ) { 
      if(jumpR) {
        setRight();
        setVelocityX( fabs( 375.0 ) );
        setVelocityY(0);
        right = true;
        jumpR = false;
        jumpL = false;
      }
      if(jumpL) {
        setLeft();
        setVelocityX( -fabs( 375.0 ) );
        setVelocityY(0);
        right = false;
        jumpR = false;
        jumpL = false;
      }
    }

 }*/
     // std::cout << "in Johnny::update()" << std::endl;