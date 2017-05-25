#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "explodingSprite.h"


class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&);
  MultiSprite(const MultiSprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual void explode();
  virtual const Frame* getFrame() const { 
    return frames->at(currentFrame); 
  }

protected:
  ExplodingSprite* explosion;
  const std::vector<Frame *>* frames;


  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);
  //Vector2f makeVelocity(int, int) const;

};
#endif
