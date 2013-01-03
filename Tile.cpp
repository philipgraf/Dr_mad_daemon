#include "Tile.h"

int tileconf[]={6,1,1,1,1,1,1}; //definiert Anzahl der Frames; Index [0] = Anzahl der Tiles
SDL_Surface *Tile::tileset;

Tile::Tile(u_int64_t id) {
	this->id = id & 0xFFFFFFFF;
	this->flags= (id - this->id) >> 16;
	currentframe = 0;
}

void Tile::render(int x, int y, SDL_Rect view) {
	Tools::drawImage(SDL_GetVideoSurface(),x*TILESIZE-view.x,y*TILESIZE-view.y,tileset,currentframe*TILESIZE,id*TILESIZE);
}

void Tile::nextFrame() {
	if(tileconf[id]!=1) {
		currentframe++;
		if(currentframe>=tileconf[id])
			currentframe = 0;
	}
}

void Tile::loadTileset() {
	tileset=Tools::loadImage("img/tiles.png");
}
