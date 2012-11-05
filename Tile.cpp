#include "Tile.h"

int tileconf[]={6,1,1,1,1,1,1}; //definiert Anzahl der Frames; Index [0] = Anzahl der Tiles
SDL_Surface *Tile::tileset;

Tile::Tile(int id) {
	this->id = id;
	currentframe = 0;
}

void Tile::render(int x, int y) {
	Tools::drawImage(SDL_GetVideoSurface(),x*TILESIZE,y*TILESIZE,tileset,currentframe*TILESIZE,id*TILESIZE);
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
