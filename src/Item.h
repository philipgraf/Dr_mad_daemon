/*
 * Item.h
 *
 *  Created on: 25.02.2013
 *      Author: philip
 */

#ifndef ITEM_H_
#define ITEM_H_

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include <Box2D/Box2D.h>

class Item {
public:
	static std::vector<Item*> itemlist; /**< contains all items */
private:
	SDL_Surface *image; /**< the image of the item */
	std::string type; /**< the type of the item */
	b2Body *body; /**< the body for collision detection */
	Uint32 timer; /**< time in milliseconds the item does not collide with the player until this is grater then 1000*/

public:
	Item(std::string name, int x, int y, int relX=0,int relY=0);
	virtual ~Item();

	void logic();

	SDL_Surface *getImage();
	SDL_Rect getClipRect();
};

#endif /* ITEM_H_ */
