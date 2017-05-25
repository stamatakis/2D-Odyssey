#ifndef SMARTSPRITE__H
#define SMARTSPRITE__H
#include <iostream>
#include "enemy.h"
#include "ioMod.h"
#include "Johnny.h"

class SmartSprite : public Enemy {
public:
  SmartSprite(const std::string&, Johnny&);
  void draw() const;
  void update(Uint32 ticks);
  void goUpLeft();
  //void goDownLeft();
  void goUpRight();
  void goDownLeft();
  // void goRight();
  // void goUp();
  // void goDown();

private:	
  IOmod& io;	
  const Johnny& enemy;
  SmartSprite(const SmartSprite &);
  SmartSprite& operator=(const SmartSprite &rhs);
};
#endif