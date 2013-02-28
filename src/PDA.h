/*
 * PDA.h
 *
 *  Created on: 19.02.2013
 *      Author: philip
 */

#ifndef PDA_H_
#define PDA_H_

#define PDA_CLOCK 0

#include "Event.h"
#include "Language.h"

#include <SDL/SDL.h>
#include <vector>


typedef struct {
	SDL_Surface *itemname;
	SDL_Surface *amound;
} items_t;

class PDA: public Event {
private:
	SDL_Surface *image;
	SDL_Surface *display;
	SDL_Surface *curser;
	SDL_Surface *timer;
	SDL_Color green;

	SDL_Rect displayRect;
	SDL_Rect imageRect;
	SDL_Rect curserRect;
	SDL_Rect timerRect;

	Language lang;

	int level;
	unsigned currentItem;
	std::vector<items_t> itemlist;

	bool running;

	void render();
	void build();

	void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void onWiiButtonEvent(int button);

public:
	PDA(int level = 0);
	virtual ~PDA();

	int show();
};

#endif /* PDA_H_ */
