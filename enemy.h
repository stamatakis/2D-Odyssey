#ifndef ENEMY__H
#define ENEMY__H
#include <string>
#include <vector>
#include <cmath>
#include "multisprite.h"
#include "explodingSprite.h"
#include "sound.h"


class Enemy : public MultiSprite {
public:
    Enemy(const std::string&);
    Enemy(const Enemy&);
    ~Enemy();
    virtual void resetEnemy();
    virtual void draw() const;
    virtual void update(Uint32 ticks);
    virtual void explode();
    virtual const Frame* getFrame() const{ 
    return frames->at(currentFrame); 
  }
    void goRight();

    void goLeft();
    bool explosionOver();


protected:
    ExplodingSprite* explosion;
    const std::vector<Frame *> fly;
    SDLSound& sound;
    Vector2f makeVelocity(int, int) const;
};
#endif