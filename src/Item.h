#ifndef ITEM_H_
#define ITEM_H_

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include <box2d/box2d.h>

/** Item class.
 * This class handles the Items.
 * @author Felix Eckner
 * @date 14.04.2013
 * @version 0.1.0 Alpha-State
 */
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
