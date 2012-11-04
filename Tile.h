#ifndef _TILE_H_
#define _TILE_H_

#include "define.h"
#include "Tools.h"

#include <SDL/SDL.h>




class Tile {
private:
	int id;
	int currentframe;
	static SDL_Surface *tileset;

public:
	Tile(int id=0);
	void render(int x,int y);
	void nextFrame();
	static void loadTileset();

	int getId() const {return id;}
};

#endif
