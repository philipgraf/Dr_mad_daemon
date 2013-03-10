#ifndef _TILE_H_
#define _TILE_H_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include "define.h"

#define TF_SWITCH1 0x1
#define TF_SWITCH2 0x2
#define TF_SWITCH3 0x4
#define TF_SWITCH4 0x8
#define TF_SWITCH5 0x10
#define TF_SWITCH6 0x20
#define TF_SWITCH7 0x40
#define TF_SWITCH8 0x80
#define TF_FINISH 0x100

class Tile {
public:
	static SDL_Surface *tileset;
private:
	u_int16_t id;
	u_int64_t flags;
	int currentframe;
	void nextFrame();

	Uint32 timer;

public:
	Tile(u_int64_t id = 0);
	static void loadTileset();

	void logic();

	//-------------------------GETTER AND SETTER ------------------------------------------//
	u_int16_t getId() const;
	int getCurrentframe() const;
	void setCurrentframe(int currentframe);
	u_int64_t getFlags() const;
	void setFlags(u_int64_t flags);
	void setId(u_int16_t id);
};

#endif
