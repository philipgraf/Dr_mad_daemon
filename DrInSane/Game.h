/*
 * game.h
 *
 *  Created on: 10.09.2012
 *      Author: philip
 */

#ifndef GAME_H_
#define GAME_H_


#include "Event.h"
#include "define.h"
#include <SDL/SDL.h>

class Game: public Event {
private:
	bool running;
	SDL_Surface *display;

public:
	Game();
	virtual ~Game();

	int execute();
	bool init();
	void onEvent(SDL_Event *event);
	void logic();
	void render();
	void cleanUp();

	void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void onExit();


};

#endif /* GAME_H_ */
