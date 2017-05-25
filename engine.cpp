#include <iostream>
#include <algorithm>
#include <random>
#include <sstream>
#include <string>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "Johnny.h"
#include "enemy.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"


class FuncLess{
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) const {
    return lhs->getScale() < rhs->getScale();
  }
};

Engine::~Engine() { 
  std::cout << "Terminating program" << std::endl;
  for(auto it : sprites) delete it;
 /* std::vector<Drawable*>::iterator it = sprites.begin();
  while ( it != sprites.end() ) {
    delete *it;
    ++it;
  }*/
  delete strategy;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  layer5("layer_5", Gamedata::getInstance().getXmlInt("layer_5/factor") ),
  layer4("layer_4", Gamedata::getInstance().getXmlInt("layer_4/factor") ),
  layer3("layer_3", Gamedata::getInstance().getXmlInt("layer_3/factor") ),
  layer2("layer_2", Gamedata::getInstance().getXmlInt("layer_2/factor") ),
  layer1("layer_1", Gamedata::getInstance().getXmlInt("layer_1/factor") ),
  huddy(),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(-1),
  makeVideo( false ),
  strategy( new PerPixelCollisionStrategy ),
  hudDraw(true),
  hudDir(true),
  Jptr(nullptr),
  sound(SDLSound::getInstance()),
  deadMoths(0),
  youWon(false),
  youlost(false)
{
  constexpr float u = 1.0f; //Mean size
  constexpr float d = 0.5f; //Std deviation

  std::random_device rd;
  std::mt19937 mt(rd());
  std::normal_distribution<float> dist(u,d);
 // sprites.push_back( new Sprite("star") );
  unsigned int n = Gamedata::getInstance().getXmlInt("numberOfmoths");
  //sprites.push_back( new Johnny("run") );
  Jptr = new Johnny("run");

  for(unsigned int i = 0; i < n; ++i){
    auto* s = new SmartSprite("moth", *Jptr);
   float scale = dist(mt);
   while(scale < 0.1f) scale = dist(mt);
   s->setScale(scale);
    sprites.push_back(s);
  }
  std::vector<Drawable*>::iterator ptr = sprites.begin();
  ++ptr;
  sort(ptr, sprites.end(), FuncLess());

  switchSprite();
  std::cout << "Loading complete" << std::endl;
}

void Engine::reset() {
   Jptr->resetJohnny();
   for(unsigned int i = 0; i < sprites.size(); ++i){
      sprites.at(i)->resetEnemy();
    }
   clock.startClock();
   // currentSprite = 0;
   Viewport::getInstance().setObjectToTrack(Jptr);

   deadMoths = 0;
   youWon = false;
   youlost = false;
   // currentSprite = 0;
   // Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}

void Engine::youWin(){
    youWon = true;

}

void Engine::youLost() {
    youlost = true;
}

void Engine::draw() const {
  //**The XML has functionality for red, green, blue, and black text, 
  //**in addition to the white we started with.
  // std::string pickColor = "black";
  layer5.draw();
  layer4.draw();
  layer3.draw();
  for(unsigned int i = 0; i < 26; ++i){
    sprites.at(i)->draw();
  }
  layer2.draw();
  for(unsigned int i = 26; i < 51; ++i){
    sprites.at(i)->draw();
  }
  layer1.draw();
  for(unsigned int i = 51; i < 75; ++i){
    sprites.at(i)->draw();
  }
             //  std::cout<<"in engine draw()" << std::endl;

  //sprites.at(0)->draw();

  Jptr->draw();
  
  if(hudDraw){
    huddy.drawHud();
  }
  if(hudDir){
    huddy.directions();
  }
  if(youWon) huddy.youWin();
  if(youlost) huddy.youLost();
   std::stringstream strm;
  //strm << sprites.at(0)->getPower();
  strm << Jptr->getPower();
  //huddy.setString(strm.str());

  std::string pickRed = "red";
  std::string printString;
  //printString.clear();
  printString.append("Health: ");
  printString.append(strm.str());
  printString.append("%");
 
  io.writeText(printString, pickRed, 760, 12);
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  Jptr->update(ticks);
  for(auto* s : sprites) s->update(ticks);
  layer5.update();
  layer4.update();
  layer3.update();
  layer2.update();
  layer1.update();
  viewport.update(); // always update viewport last
  unsigned int sec;
  sec = clock.getSeconds();
  if(4 < sec && sec < 6) {
     hudDraw = false;
  }
  if(4 < sec && sec < 6) {
     hudDir = false;
  }
  
  // std::stringstream strm;
  // //strm << sprites.at(0)->getPower();
  // strm << Jptr->getPower();
  // huddy.setString(strm.str());
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(Jptr);
}

void Engine::checkForCollisions() {
  //std::vector<Drawable*>::const_iterator it = sprites.begin();
  unsigned int i = 51;
  //Drawable* player = sprites[0];
  //++it;
  while ( i < sprites.size() ) {
    if(Jptr->collidedWith(sprites[i])){
       sprites[i]->explode();
       sound[0];
       ++deadMoths;
    }
    ++i;
  }
  if(Jptr->youLose()){
    youLost();
  }
  if(deadMoths > 10){
     youWin();
   }
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;


  while ( !done ) {
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_SPACE] ) {
          Jptr->shoot();
         // sprites.at(currentSprite)->shoot();
        }
        if ( keystate[SDL_SCANCODE_Z] ) {
          clock.toggleSloMo();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_DOWN] && keystate[SDL_SCANCODE_UP]) {
          Jptr->stop();
         // sprites.at(currentSprite)->stop();
        }
        else if(keystate[SDL_SCANCODE_DOWN] && keystate[SDL_SCANCODE_RIGHT]){
           Jptr->stop();
          //sprites.at(currentSprite)->stop();
        }
        else if(keystate[SDL_SCANCODE_DOWN] && keystate[SDL_SCANCODE_LEFT]){
           Jptr->stop();
          //sprites.at(currentSprite)->stop();
        }
        else if(keystate[SDL_SCANCODE_S] && keystate[SDL_SCANCODE_W]){
           Jptr->stop();
          //sprites.at(currentSprite)->stop();
        }
        else if(keystate[SDL_SCANCODE_S] && keystate[SDL_SCANCODE_A]){
           Jptr->stop();
          //sprites.at(currentSprite)->stop();
        }
        else if(keystate[SDL_SCANCODE_S] && keystate[SDL_SCANCODE_D]) {
           Jptr->stop();
          //sprites.at(currentSprite)->stop();
        }
        else if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]) {
           Jptr->stop();
          //sprites.at(currentSprite)->stop();
        }
        else if ((keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_RIGHT]) ||
           (keystate[SDL_SCANCODE_W] &&  keystate[SDL_SCANCODE_D])) {
              Jptr->setJumpR(true);
              Jptr->jumpLR();

              //sprites.at(currentSprite)->setJumpR(true);
              //sprites.at(currentSprite)->jumpLR();
            //  std::cout<< "in the loop after jumpLR right" << std::endl;
        }
        else if ((keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_LEFT]) ||
           (keystate[SDL_SCANCODE_W] &&  keystate[SDL_SCANCODE_A])) {
              Jptr->setJumpR(false);
              Jptr->jumpLR();
                                   //   std::cout<< "in the loop after jumpLR left" << std::endl;

        }
        else if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]) {
          Jptr->goRight();
                      //  std::cout<< "in the loop after goright" << std::endl;

        }
        else if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A]) {
          Jptr->goLeft();
        }
        else if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W]) {
          Jptr->jumpUp();
        }
        if (keystate[SDL_SCANCODE_R]){
          reset();
        }
        if (keystate[SDL_SCANCODE_M]){
          sound.toggleMusic();
        }
        if (keystate[SDL_SCANCODE_G]){
          Jptr->setGod();
        }
        if (keystate[SDL_SCANCODE_F1] && !hudDraw ) {
            hudDraw = true;
        }
        else if(keystate[SDL_SCANCODE_F1] && hudDraw ) {
            hudDraw = false;
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
        
      }
    } 
   // 
    ticks = clock.getElapsedTicks();
    //std::cout<< ticks << std::endl;
    if ( ticks > 0 ) {
      clock.incrFrame();

      draw();
      update(ticks);
     // huddy.clearString();

      checkForCollisions();
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
