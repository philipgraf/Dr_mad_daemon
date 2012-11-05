#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "Tile.h"
#include "Tools.h"

#include <fstream>
#include <iostream>
#include <SDL/SDL.h>

using namespace std;

class Level {
private:
	/**
	 * 3D Tilearray [0=bg,1=main,2=fg][x][y]
	 */
	Tile ****tilelist;
	string name;
	SDL_Surface *background;
	int width;
	int height;
	int gravity;
	int time;
	
public:
	Level(string lname="l000");
	~Level();
	int getGravity() const;
	void setGravity(int gravity);
	const string& getName() const;
	int getTime() const;
	void setTime(int time);
	int getTileID(int x,int y,int layer=1);
	void render(SDL_Rect view,int layer=1);
};


#endif
