#ifndef _TILE_H_
#define _TILE_H_

#include <SDL/SDL.h>

#include "Tools.h"

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
	Tile(u_int64_t id=0);
	static void loadTileset();

	void logic();




	//-------------------------GETTER AND SETTER ------------------------------------------//
	u_int16_t getId() const {
		return id;
	}

	int getCurrentframe() const
	{
		return currentframe;
	}

	void setCurrentframe(int currentframe)
	{
		this->currentframe = currentframe;
	}

	u_int64_t getFlags() const
	{
		return flags;
	}

	void setFlags(u_int64_t flags)
	{
		this->flags = flags;
	}

	bool isTransparency() const
	{
		return transparency;
	}

	void setTransparency(bool transparency)
	{
		this->transparency = transparency;
	}
};

#endif
