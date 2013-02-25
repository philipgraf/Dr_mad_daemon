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
	static std::vector<Item*> itemlist;
private:
	SDL_Surface *image;
	std::string type;
	b2Body *body;
	Uint32 timer;
public:
	Item(std::string name, int x, int y, int relX=0,int relY=0);
	virtual ~Item();

	void logic();

	std::string getType();
	SDL_Surface *getImage();
	SDL_Rect getClipRect();
};

#endif /* ITEM_H_ */
