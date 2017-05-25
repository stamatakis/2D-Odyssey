#include "Johnny.h"
#include "gamedata.h"
#include "renderContext.h"


/*CollisionStrategy* getStrategy(const string& name) {
  std::string sName = Gamedata::getInstance().getXmlStr(name+"/strategy");
  if ( sName == "midpoint" ) return new MidPointCollisionStrategy;
  if ( sName == "rectangular" ) return new RectangularCollisionStrategy;
  if ( sName == "perpixel" ) return new PerPixelCollisionStrategy;
  throw std::string("No strategy in getStrategy");
}*/

Johnny::Johnny(const std::string& name) :
   MultiSprite(name),
   clock(Clock::getInstance()),
   strategy(),
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
   bullets(BulletPool::getInstance() ),
   minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
   power(1000),
   god(false),
   youlost(false)
   
   {
    bullets.setName(bulletName);
    frames = &run;
    numberOfFrames = Gamedata::getInstance().getXmlInt(name+"/frames");
    frameInterval = Gamedata::getInstance().getXmlInt(name+"/frameInterval");
    frameWidth = run[0]->getWidth();
    frameHeight = run[0]->getHeight();

   }


Johnny::Johnny(const Johnny& s) :
   MultiSprite(s), 
   clock(s.clock),
   strategy(s.strategy),
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
   minSpeed(s.minSpeed),
   power(s.power),
   god(s.god),
   youlost(s.youlost)
   {}

void Johnny::resetJohnny() {
       Vector2f resetPos =Vector2f(Gamedata::getInstance().getXmlInt("run/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt("run/startLoc/y"));
       setPosition(resetPos);
       power = 1000;
}

void Johnny::shoot() { 

  float x = getX()+getFrame()->getWidth();
  float y = getY()+getFrame()->getHeight()/2;
  Vector2f speed;
  Vector2f posit; 
  if(getVelocityX() > 0 || getVelocityX() == 0) {
    posit = Vector2f(x, y);
    speed = Vector2f(minSpeed+getVelocityX(), 0);

  }
  else{
    float negSpeed = -minSpeed+getVelocityX();
    float z = getX();
    posit = Vector2f(z,y);
    speed = Vector2f(negSpeed, 0);
  }
  // I'm not adding minSpeed to y velocity:
  bullets.shoot( posit, speed );

}  

bool Johnny::collidedWith(const Drawable* obj) {
  if ( strategy.execute(*this, *obj) ) {
    {
      if(!god){
      --power;
      if(power < 1) {
         MultiSprite::explode();
         power = 1000;
      }
     }
    }
  }
  return bullets.collidedWith( obj );
}

void Johnny::draw() const { 

  //frames->at(currentFrame)->draw(getX(), getY(), getScale());
  MultiSprite::draw();  
         //   std::cout << "in Johnny.draw()" << std::endl;

  bullets.draw();

}
/*void Johnny::draw() const { 
  frames->at(currentFrame)->draw(getX(), getY());
}*/

void Johnny::setRight() {
    right = true;
    frames = &run;
    currentFrame = 0;
    numberOfFrames = Gamedata::getInstance().getXmlInt("run/frames");
    frameInterval = Gamedata::getInstance().getXmlInt("run/frameInterval");
    frameWidth = run[0]->getWidth();
    frameHeight = run[0]->getHeight();
}
void Johnny::setLeft() {
    right = false;
    frames = &runLeft;
    currentFrame = 0;
    numberOfFrames = Gamedata::getInstance().getXmlInt("run/frames");
    frameInterval = Gamedata::getInstance().getXmlInt("run/frameInterval");
    frameWidth = run[0]->getWidth();
    frameHeight = run[0]->getHeight();
}
void Johnny::jumpUp(){
  if(!jumpbool) {
        frames = &jump;
        currentFrame = 0;
        setVelocityX( fabs(0.0) );
        setVelocityY( -fabs( 500.0 ) ); 

        numberOfFrames = numberOfJump;
        frameInterval = jumpInterval;
        frameWidth = jumpWidth;
        frameHeight = jumpHeight;
    
        right = true;
        jumpR = true;
        jumpL = false;
        jumpStart = clock.getSeconds();
        jumpbool = true;

   }
        // std::cout << "right: " << right << std::endl;
        // std::cout << "jumpR: " << jumpR << std::endl;
        // std::cout << "jumpL: " << jumpL << std::endl;
    
}
void Johnny::jumpLR(){
  // if (jumpbool == false){
     //std::cout << "Johnny::jumpLR() top" << std::endl;
  if(!jumpbool) {
     if(jumpR){
        right = true;
        jumpL = false;
        frames = &jump;
        setVelocityX( fabs( 675.0 ) );
        setVelocityY( -fabs( 500.0 ) ); 
     }
     if(!jumpR){
        right = false;
        jumpL = true;
        frames = &jumpLeft;
        setVelocityX( -fabs( 675.0 ) );
        setVelocityY( -fabs( 500.0 ) );
     }
     currentFrame = 0;
     numberOfFrames = numberOfJump;
     frameInterval = jumpInterval;
     frameWidth = jumpWidth;
     frameHeight = jumpHeight;
     jumpStart = clock.getSeconds();
     jumpbool = true;
  }  
}
void Johnny::goRight() {
  jumpbool = false;
  if(right == false)
     setRight();
  setVelocityX( fabs( 475.0 ) );
  //setVelocityY(0);
  jumpR = false;
  jumpL = false;
}

void Johnny::goLeft() {
  jumpbool = false;
  if(right == true)
     setLeft();
  setVelocityX( -fabs( 475.0 ) );
  //setVelocityY(0);
  right = false;
  jumpR = false;
  jumpL = false;
}

void Johnny::stop() {
    jumpbool = false;
    frames = &run;
    numberOfFrames = Gamedata::getInstance().getXmlInt("run/frames");
    frameInterval = Gamedata::getInstance().getXmlInt("run/frameInterval");
    frameWidth = run[0]->getWidth();
    frameHeight = run[0]->getHeight();
  setVelocityX(0);
  //setVelocityY(0);
  right = true;
  jumpR = false;
  jumpL = false;
}

void Johnny::update(Uint32 ticks) { 
  if(explosion) MultiSprite::update(ticks);
  else{
    advanceFrame(ticks);
    bullets.update(ticks);
    
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    setPosition(getPosition() + incr);
    if(getVelocityY() == 0) jumpbool = false;
    if ( getY() > worldHeight-frameHeight) { 
      setVelocityY( 0 );
      setY(worldHeight-frameHeight);
      if(right){
        setRight();
        right = true;
        jumpR = false;
        jumpL = false;
      }
      else{
         setLeft();
         right = false;
         jumpR = false;
         jumpL = false;
      }
    }
    if ( getY() < worldHeight-(frameHeight*4)) {
     // if(getY < worldHeight-frameHeight)
        setVelocityY( fabs ( getVelocityY() + GRAVITY ));
    }
    if ( getY() < 0) {
      setVelocityY( fabs( getVelocityY() ) );
    }
    if ( getX() < 0) {
      setVelocityX( fabs( getVelocityX() ) );
      setRight();
      right = true;
      jumpR = false;
      jumpL = false;
    }
    if ( getX() > worldWidth-frameWidth) {
      setVelocityX( -fabs( getVelocityX() ) );
      setLeft();
      right = false;
      jumpR = false;
      jumpL = false;
    }  
  }
}
