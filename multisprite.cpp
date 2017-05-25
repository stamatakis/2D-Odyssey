#include "multisprite.h"
#include "gamedata.h"
#include "renderContext.h"


void MultiSprite::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  explosion(nullptr),
  frames( ),
  currentFrame(0),
  numberOfFrames( 0 ),
  frameInterval( 0),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(0),
  frameHeight(0)
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s), 
  explosion(s.explosion),
  frames(s.frames),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }

// MultiSprite& MultiSprite::operator=(const MultiSprite& s) {
//   Drawable::operator=(s);
//   frames = s.frames;
//   currentFrame = s.currentFrame;
//   numberOfFrames = s.numberOfFrames;
//   frameInterval = s.frameInterval;
//   timeSinceLastFrame = s.timeSinceLastFrame;
//   worldWidth = s.worldWidth;
//   worldHeight = s.worldHeight;
//   frameWidth = s.frameWidth;
//   frameHeight = s.frameHeight;
// }
inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}
  
void MultiSprite::draw() const { 
  if (explosion) {
    explosion->draw();
    return;
  }
  else{
    if(getScale() < SCALE_EPSILON) return;
    frames->at(currentFrame)->draw(getX(), getY(), getScale());
  }
}

void MultiSprite::explode() { 
  if ( explosion ) return;
  Sprite sprite(getName());
  Vector2f myPos = getPosition();
  sprite.setPosition(myPos);
  explosion = new ExplodingSprite(sprite);
}

void MultiSprite::update(Uint32 ticks) { 
  if ( explosion ) {
     explosion->update(ticks);
     if ( explosion->chunkCount() == 0 ) {
        delete explosion;
        explosion = NULL;
     }
     return;
  }
  else{
    advanceFrame(ticks);

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
    }
    if ( getX() > worldWidth-frameWidth) {
      setVelocityX( -fabs( getVelocityX() ) );
    }  
  }
}
