#ifndef TWOWAYMULTI__H
#define TWOWAYMULTI__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class TwoWayMulti : public MultiSprite {
public:
	TwoWayMulti(const std::string&);
	TwoWayMulti(const TwoWayMulti&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const;

};
#endif