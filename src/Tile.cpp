#include "Tile.h"


const int tileconf[] = {180, 1, 1, 1, 1, 1, 1, 1, 10, 1, 1,
						10, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
						2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
						2, 2, 2, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1
						}; /**< defines the quantity of frames for each tile; index 0 is the total number of tiles  */
unsigned tileduration[] = {1337,  1, 1, 1, 1, 1, 1, 1, 250, 1, 1,
						40, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1000,
						1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
						1000, 1000, 1000, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1
						}; /**< defines the time between framechanges for each tile in milliseconds ; index 0 is simply 1337   */

SDL_Surface *Tile::tileset;

/** Constructor of tile.
 * Split the given Value in ID (last 16bit) and FLAGS (first 48bit), set the first frame and the time
 *
 * @param id the number got from the .map file which contains flags and ID
 */
Tile::Tile(Sint64 id) {
	this->id = id & 0xFFFF;
	this->flags = id >> 16;
	currentframe = 0;

	timer = SDL_GetTicks();
}

/** Changes the frame of a tile.
 * If the duration the current frame should be displayed is over it will be set to the next or the first (if the current frame is the last)
 *
 * @see tileconf
 * @see tileduration
 */
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

/** Loads the image and convert it to a SDL_Surface.
 * The image includes "textures" of all tiles (top to bottom) and frames (left to) right.
 * also a colorkey is set to make a defined color(0xFF00FF) transparent as .bmp do not provide an alpha channel but give the best performance
 */
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

/** Executes the logic of the tile.
 * Actually only calls nextFrame
 *
 * @see nextFrame()
 */
void Tile::logic() {
	nextFrame();
}

//-------------------------GETTER AND SETTER ------------------------------------------//
/**
 * Returns the ID of the tile.
 *
 * @see id
 */
Sint16 Tile::getId() const {
	return id;
}

/**
 * Returns the current frame of the tile.
 *
 * @see currentframe
 */
int Tile::getCurrentframe() const {
	return currentframe;
}

/**
 * Sets the current frame of the tile.
 * This is used to change the appearance of the tile. In example when a switch is used
 *
 * @param currentframe the number of the frame that should be rendered
 */
void Tile::setCurrentframe(int currentframe) {
	this->currentframe = currentframe;
}

/**
 * Returns the flags of the tile.
 *
 * @see flags
 */
Sint64 Tile::getFlags() const {
	return flags;
}

/**
 * Sets a new id for this Tile.
 * this can be used to alter a level while running
 *
 * @param id the new id this tile should have from now on
 *
 * @see id
 */
void Tile::setId(Sint16 id) {
	this->id = id;
}

/**
 * Sets new flags to the tile.
 * This can be used to create a new finishpoint while running
 *
 * @param flags
 *
 * @see flags
 */
void Tile::setFlags(Sint64 flags) {
	this->flags = flags;
}
