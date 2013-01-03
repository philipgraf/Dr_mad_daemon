
#include "includes.h"

#ifndef _TILE_H_
#define _TILE_H_

#define TF_START 0x10000
#define TF_FINISH 0x20000


class Tile {
private:
	u_int16_t id;
	u_int64_t flags;
	int currentframe;
	static SDL_Surface *tileset;

public:
	Tile(u_int64_t id=0);
	void render(int x,int y, SDL_Rect view);
	void nextFrame();
	static void loadTileset();

	u_int16_t getId() const {return id;}

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
};

#endif
