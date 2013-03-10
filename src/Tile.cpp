#include "Tile.h"

//TODO: tileFrames get from file and not globle
int tileconf[] = { 90,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						}; //definiert Anzahl der Frames; Index [0] = Anzahl der Tiles
unsigned tileduration[] = {  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             };

SDL_Surface *Tile::tileset;

Tile::Tile(u_int64_t id) {
	this->id = id & 0xFFFF;
	this->flags = id >> 16;
	currentframe = 0;

	timer = SDL_GetTicks();

}

void Tile::nextFrame() {
	if (id != 0 && tileconf[id] != 1) {
		if (SDL_GetTicks() - timer > tileduration[id]) {
			currentframe++;
			timer = SDL_GetTicks();
		}

		if (currentframe >= tileconf[id])
			currentframe = 0;
	}
}

void Tile::loadTileset() {

	SDL_Surface *tmp = SDL_LoadBMP(IMG"tiles.bmp");
	if(!tmp){
		//TODO Throw exception
		std::cout << "unable to load tiles.bmp" << std::endl;
	}
	else {
		tileset = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		if (tileset != 0) {
			SDL_SetColorKey(tileset, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(tileset->format, 255, 0, 255));
		}
	}

}

void Tile::logic() {
	nextFrame();
}

//-------------------------GETTER AND SETTER ------------------------------------------//
u_int16_t Tile::getId() const {
	return id;
}

int Tile::getCurrentframe() const {
	return currentframe;
}

void Tile::setCurrentframe(int currentframe) {
	this->currentframe = currentframe;
}

u_int64_t Tile::getFlags() const {
	return flags;
}

void Tile::setId(u_int16_t id) {
	this->id = id;
}

void Tile::setFlags(u_int64_t flags) {
	this->flags = flags;
}
