#include "Tile.h"

//TODO: tileFrames get from file and not globle
int tileconf[]={8,1,1,1,1,1,1,9,1}; //definiert Anzahl der Frames; Index [0] = Anzahl der Tiles
SDL_Surface *Tile::tileset;

Tile::Tile(u_int64_t id) {
	this->id = id & 0xFFFF;
	this->flags= id >> 16;
	currentframe = 0;
	transparency=false;

}

void Tile::nextFrame() {
	if(id!=0 && tileconf[id]!=1) {
		currentframe++;
		if(currentframe>=tileconf[id])
			currentframe = 0;
	}
}

void Tile::loadTileset() {
	SDL_Surface *tmp = SDL_LoadBMP(IMG"tiles.bmp");

	if(!tmp){
		cout << "unable to load BMP file" << endl;
	}else{
		tileset = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		if(tileset!=0){
			SDL_SetColorKey(tileset,SDL_SRCCOLORKEY| SDL_RLEACCEL,SDL_MapRGB(tileset->format,255,0,255));

		}
	}


	//tileset=Tools::loadImage(IMG"tiles.png");
}

void Tile::logic() {
	nextFrame();
}
