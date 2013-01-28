/*
 * game.h
 *
 *  Created on: 10.09.2012
 *      Author: philip
 */

#ifndef GAME_H_
#define GAME_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "Level.h"
#include "Menu.h"
#include "Event.h"

using namespace std;

class Game: public Event {

public:
	static Game *curGame;
private:
	bool running;
	SDL_Surface * display;
	TTF_Font *font;

	Level *currentLevel;

public:
	Game();
	virtual ~Game();

	int execute();
	void init();
	void onEvent(SDL_Event *event);
	void logic();
	void render();
	void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void onKeyUP(SDLKey sym, SDLMod mod, Uint16 unicode);
	void cleanUp();


	void onExit();

	Level* getCurrentLevel(){
		return currentLevel;
	}

	void setCurrentLevel(Level *curLev){
		currentLevel = curLev;
	}

	void setRunning(bool r){
		running =r;
	}

	bool isRunning(){
		return running;
	}

	TTF_Font* getFont(){
		return font;
	}



};


#endif /* GAME_H_ */
