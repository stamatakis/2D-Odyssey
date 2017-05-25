#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "smartSprite.h"
#include "Johnny.h"
#include "sound.h"

class CollisionStrategy;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();
  void youWin();
  void youLost();

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World layer5;
  World layer4;
  World layer3;
  World layer2;
  World layer1;
  Hud huddy;
  Viewport& viewport;

  std::vector<Drawable*> sprites;

  int currentSprite;
  bool makeVideo;
  CollisionStrategy* strategy;
  bool hudDraw;
  bool hudDir;
  Johnny* Jptr;
  SDLSound& sound;
  int deadMoths;
  bool youWon;
  bool youlost;


  void reset();

  void draw() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void checkForCollisions();

};
