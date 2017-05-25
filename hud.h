#include <string>
#include <vector>
#include <map>
#include "frame.h"
#include "ioMod.h"
#include "clock.h"
#include "bulletPool.h"



class Hud {
public:
	Hud();
	~Hud();
	void setString(const std::string&);
	/*void clearString() { 
		printString.clear();
	   
	}*/
	void drawHud() const;
	void youWin() const;
	void youLost() const;
	void directions() const;

private:
	SDL_Renderer* const rend;
    const IOmod& io;
    Clock& clock;
    BulletPool& pool;
	SDL_Rect r;
	std::string printString;
};