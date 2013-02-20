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

#include <SDL/SDL.h>

class PDA : public Event {
private:
	SDL_Surface *image;
	SDL_Surface *display;
	SDL_Surface *curser;
	SDL_Color green;

	int level;
	int currentItem;

	bool running;

	void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);

public:
	PDA(int level=0);
	virtual ~PDA();

	int show();
};

#endif /* PDA_H_ */
