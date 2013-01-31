#ifndef _TILE_H_
#define _TILE_H_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include "define.h"

#define TF_START 0x10000
#define TF_FINISH 0x20000

class Tile {
private:
	u_int16_t id;
	u_int64_t flags;
	int currentframe;
	void nextFrame();
	bool transparency;

public:
	static SDL_Surface *tileset;
	Tile(u_int64_t id = 0);
	static void loadTileset();

	void logic();

	//-------------------------GETTER AND SETTER ------------------------------------------//
	u_int16_t getId() const;

	int getCurrentframe() const;

	void setCurrentframe(int currentframe);

	u_int64_t getFlags() const;

	void setFlags(u_int64_t flags);

	bool isTransparency() const;

	void setTransparency(bool transparency);
};

#endif
