#ifndef Johnny__H
#define Johnny__H
#include <string>
#include <vector>
#include <cmath>
#include "multisprite.h"
#include "clock.h"
#include "bulletPool.h"
#include "collisionStrategy.h"


const float GRAVITY = .5; 

class Johnny : public MultiSprite {
public:
    Johnny(const std::string&);
    Johnny(const Johnny&);
   // ~Johnny() { delete strategy;}

    virtual void draw() const;
    virtual void update(Uint32 ticks);
    virtual const Frame* getFrame() const{ 
    return frames->at(currentFrame); 
  }
    int getPower() { return power/10; }
    void jumpUp();
    void jumpLR();
    void setJumpR(bool i) { jumpR = i; }
    void goRight();

    void goLeft();
    void stop();
    void shoot();
    virtual bool collidedWith(const Drawable*);
    void setRight();
    void setLeft();
    void setGod() { god = !god; }
    void resetJohnny();
    bool youLose() {return youlost;}

protected:
    Clock& clock;
    PerPixelCollisionStrategy strategy;
    const std::vector<Frame *> run;
    const std::vector<Frame *> runLeft;
    const std::vector<Frame *> jump;
    const std::vector<Frame *> jumpLeft;

    //unsigned numberOfFrames;
    unsigned numberOfJump;
    //unsigned frameInterval;
    unsigned jumpInterval;
   // int frameWidth;
   // int frameHeight;
    int jumpWidth;
    int jumpHeight;
    bool right;
    bool jumpbool;
    bool jumpR;
    bool jumpL;
    
    unsigned int jumpStart;
    unsigned int jumpEnd;
    std::string bulletName;
    BulletPool& bullets;
    float minSpeed;
    int power;
    bool god;
    bool youlost;

    Johnny& operator=(const Johnny&);
};
#endif