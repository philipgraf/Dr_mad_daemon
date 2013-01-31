/*
 * Menu.h
 *
 *  Created on: 28.01.2013
 *      Author: philip
 */

#ifndef MENU_H_
#define MENU_H_

#define SIZE(x) (sizeof(x)/sizeof(x[0]))

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <vector>

#include "define.h"
#include "Event.h"
#include "Game.h"


using namespace std;

//typedef void (Game::*fptr)();
typedef struct {
	string labelText;
	SDL_Surface *labelSurface;
	SDL_Rect position;
	bool selected;
	//fptr action;
} menuitem;

class Menu: public Event {
	typedef void (Menu::*fptr)();
private:
	vector<fptr> labelactions;
	vector<string> labeltexts;
	SDL_Surface *menubackground;
	menuitem *items;
	unsigned int currentItem;
	SDL_Color colors[2];

	bool running;
	void onExit();
	void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void render();

public:
	Menu();
	virtual ~Menu();

	void mStart();
	void mExit();
	void mAudio();

};



#endif /* MENU_H_ */
