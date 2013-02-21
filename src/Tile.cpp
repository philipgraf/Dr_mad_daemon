#include "Tile.h"

//TODO: tileFrames get from file and not globle
int tileconf[] = { 8, 1, 1, 1, 1, 1, 1, 9, 1 }; //definiert Anzahl der Frames; Index [0] = Anzahl der Tiles
SDL_Surface *Tile::tileset;

Tile::Tile(u_int64_t id) {
	this->id = id & 0xFFFF;
	this->flags = id >> 16;
	currentframe = 0;
	transparency = false;

}

void Tile::nextFrame() {
	if (id != 0 && tileconf[id] != 1) {
		currentframe++;
		if (currentframe >= tileconf[id])
			currentframe = 0;
	}
}

void Tile::loadTileset() {

	SDL_Surface *temp;
	if ((temp = IMG_Load(IMG"tiles.png")) == NULL) {
		//TODO Throw exception
		std::cout << "unable to load tiles.png" << std::endl;
	}
	tileset = SDL_DisplayFormatAlpha(temp);
	SDL_FreeSurface(temp);

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

void Tile::setFlags(u_int64_t flags) {
	this->flags = flags;
}

bool Tile::isTransparency() const {
	return transparency;
}

void Tile::setTransparency(bool transparency) {
	this->transparency = transparency;
}
