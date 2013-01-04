#include "Tile.h"

int tileconf[]={8,1,1,1,1,1,1,9,1}; //definiert Anzahl der Frames; Index [0] = Anzahl der Tiles
SDL_Surface *Tile::tileset;

Tile::Tile(u_int64_t id) {
	this->id = id & 0xFFFFFFFF;
	this->flags= (id - this->id) >> 16;
	currentframe = 0;
}

void Tile::nextFrame() {
	if(id!=0 && tileconf[id]!=1) {
		currentframe++;
		if(currentframe>=tileconf[id])
			currentframe = 0;
	}
}

void Tile::loadTileset() {
	tileset=Tools::loadImage("img/tiles.png");
}

void Tile::logic() {
	nextFrame();
}
