#include <iostream>
#include <SDL2/SDL.h>
#include <sstream>
#include "renderContext.h"
#include "hud.h"
#include "gamedata.h"

Hud::Hud():
    rend(RenderContext::getInstance()->getRenderer()), 
    io( IOmod::getInstance() ),
    clock( Clock::getInstance() ),
    pool(BulletPool::getInstance() ),
    printString(" ")
    {
    SDL_SetRenderDrawColor( rend, 208, 209, 210, 255 );

    // Clear winow
    SDL_RenderClear( rend);

    r.x = 7;
    r.y = 7;
    r.w = 200;
    r.h = 175;
}

Hud::~Hud(){}

void Hud::setString(const std::string& s) {
  std::string pickRed = "red";

  printString.clear();
  printString.append("Health: ");
  printString.append(s);
  printString.append("%");
 
  io.writeText(printString, pickRed, 300, 12);

  
}

void Hud::youWin() const{
  io.writeText("YOU WIN", "green", 400, 48);
  clock.pause();

}

void Hud::youLost() const {
  io.writeText("YOU LOST", "red", 400, 48);
  clock.pause();

}

void Hud::directions() const {
  io.writeText("Kill more than 10 Moths before you die", "green", 380, 48);
}

void Hud::drawHud() const {
    // First set the blend mode so that alpha blending will work;
    // the default blend mode is SDL_BLENDMODE_NONE!
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( rend, 255, 255, 255, 255/2 );

    // Render rect
    SDL_RenderFillRect( rend, &r );

    SDL_SetRenderDrawColor( rend, 173, 117, 28, 255 );
    SDL_RenderDrawRect( rend, &r );

    // Render the rect to the screen
    SDL_RenderPresent(rend);
    std::string pickColor = "black";
    std::string pickWhite = "white";
    std::string pickRed = "red";
    std::stringstream avgF;
  std::stringstream strm;
  strm << "elapsed seconds: " << clock.getSeconds();
  avgF << "avgFps: " << clock.getAvgFps();
  //strm << Gamedata::getInstance().getXmlStr("screenTitle")
  
  io.writeText(avgF.str(), pickColor, 12, 12);
  io.writeText(strm.str(), pickColor, 12, 30);
  io.writeText("'w' or 'up arrow' to jump", pickColor, 12, 48);
  io.writeText("'a' or 'left arrow' to run left", pickColor, 12, 65);
  io.writeText("'d' or 'right arrow' to run right", pickColor, 12, 82);
  io.writeText("'s' or 'down arrow' to stop", pickColor, 12, 100);
  io.writeText("F1 to toggle HUD", pickColor, 12, 157);
 // io.writeText(printString, pickRed, 12, 175);
  //io.writeText(Gamedata::getInstance().getXmlStr("screenTitle"), pickWhite,
  //  12, 460);
  pool.hudDraw();
    }
   
/*
    SDL_Event event;
    const Uint8* keystate;
    while ( true ) {
      keystate = SDL_GetKeyboardState(0);
      if (keystate[SDL_SCANCODE_ESCAPE]) { break; }
      if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          break;
        }
      }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;*/

