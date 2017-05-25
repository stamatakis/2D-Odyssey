#include "enemy.h"
#include "gamedata.h"
#include "renderContext.h"
#include "vector2f.h"

const float PI = 4.0f*std::atan(1.0f);

using RADRAND_t = decltype(std::bind(std::declval<std::uniform_real_distribution<float> >(),std::declval<std::mt19937>()));
using NORRAND_t = decltype(std::bind(std::declval<std::normal_distribution<float> >(),std::declval<std::mt19937>()));

std::mt19937 getRand(){
  static std::random_device rd;
  return std::mt19937(rd());
}

RADRAND_t getRadianDist(){
  return std::bind(std::uniform_real_distribution<float>(0.0f,2.0f*PI),getRand());
}

NORRAND_t getNormalDist(float u, float dev){
  return std::bind(std::normal_distribution<float>(u,dev),getRand());
}

Vector2f Enemy::makeVelocity(int vx, int vy) const {
  static auto rad = getRadianDist();
  auto nor = getNormalDist(vx,vy);

  float v_rad = rad();
  float v_mag = nor();

  return v_mag*Vector2f(std::cos(v_rad),std::sin(v_rad));
}


Enemy::Enemy(const std::string& name) :
   MultiSprite(name),
   explosion(nullptr),
   fly( RenderContext::getInstance()->getFrames(name) ),
   sound(SDLSound::getInstance())   
   {
    frames = &fly;
    numberOfFrames = Gamedata::getInstance().getXmlInt(name+"/frames");
    frameInterval = Gamedata::getInstance().getXmlInt(name+"/frameInterval");
    frameWidth = fly[0]->getWidth();
    frameHeight = fly[0]->getHeight();
    float px = rand() % 5000;
    float py = rand() % 576;
    Vector2f newpos(px, py);
    setPosition(newpos);
    setVelocity((makeVelocity(
                    Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")) ));
    
   }


 Enemy::Enemy(const Enemy& s) :
   MultiSprite(s),
   explosion(s.explosion),
   fly(s.fly),
   sound(s.sound)
 {}

Enemy::~Enemy() {
  delete explosion;
}

void Enemy::resetEnemy() {
    float px = rand() % 5000;
    float py = rand() % 576;
    Vector2f newpos(px, py);
    setPosition(newpos);
    setVelocity((makeVelocity(
                    Gamedata::getInstance().getXmlInt("moth/speedX"), 
                    Gamedata::getInstance().getXmlInt("moth/speedY")) ));
}

void Enemy::draw() const { 
  if (explosion) {
    explosion->draw();
    return;
  }
  else{
    frames->at(currentFrame)->draw(getX(), getY(), getScale());
  }
}

void Enemy::explode() { 
  if ( explosion ) return;
  Sprite sprite(getName());
  Vector2f myPos = getPosition();
  sprite.setPosition(myPos);
  explosion = new ExplodingSprite(sprite);
 // sound[0];
}

bool Enemy::explosionOver(){
  if(explosion == NULL)
    return true;
  else
    return false;
}
void Enemy::goRight() {
  frames = &fly;
  setVelocityX( fabs( getVelocityX() ) );
}

void Enemy::goLeft() {
//  frames = &runLeft;
  setVelocityX( -fabs( getVelocityX() ) );
}
void Enemy::update(Uint32 ticks) { 
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
      frames = &fly;
    }
    if ( getX() > worldWidth-frameWidth) {
      setVelocityX( -fabs( getVelocityX() ) );
     // frames = &runLeft;
    }  
}
}
