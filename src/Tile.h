#ifndef _TILE_H_
#define _TILE_H_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <box2d/box2d.h>
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

/** Tile class.
 * This class will define the Tiles a Level is build of
 * @author Felix Eckner
 * @date 14.04.2013
 * @version 0.1.0 Alpha-State
 */
class Tile {
public:
	static SDL_Surface *tileset; /**< This is the pointer to a BIG picture that includes all Tiles */
private:
	Sint16 id; /**< This is the id of a tile which defines the look*/
	Sint64 flags; /**< Stores the flags of a tile which are used for logical things like where is the finish or a switch*/
	int currentframe; /**< Defines the frame of a tile that will be rendered. Standard is 0 and will be altered if the tile is animated or changed by logic like a switch*/
	void nextFrame();

	Uint32 timer; /**< Time in milliseconds when the current frame was set. This is needed to change the frame after the defined time for the tile to make the animation smooth */
	b2Body *body;

public:
	Tile(Sint64 id = 0);
	static void loadTileset();

	void logic();

	//-------------------------GETTER AND SETTER ------------------------------------------//
	Sint16 getId() const;
	int getCurrentframe() const;
	void setCurrentframe(int currentframe);
	Sint64 getFlags() const;
	void setFlags(Sint64 flags);
	void setId(Sint16 id);
	b2Body* getBody();
	void setBody(b2Body* body);
};

#endif
