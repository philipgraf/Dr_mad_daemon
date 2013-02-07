/*
 * Menu.h
 *
 *  Created on: 28.01.2013
 *      Author: philip
 */

#ifndef MENU_H_
#define MENU_H_

#define SIZE(x) (sizeof(x)/sizeof(x[0]))

#define MAINMENU 0
#define SLOTMENU 1
#define OPTIONMENU 2
#define PAUSEMENU 3
#define CREDITS 4
#define LEVELMENU 5

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "define.h"
#include "Event.h"
#include "Game.h"
#include "Level.h"
#include "Language.h"

class Level;
using namespace std;

typedef struct {
	string labelText;
	SDL_Surface *labelSurface;
	SDL_Rect position;
	bool selected;
} menuitem;

class Menu: public Event {
	typedef void (Menu::*fptr)();
private:
	vector<fptr> labelactions;
	vector<string> labeltexts;
	SDL_Surface *background;
	SDL_Surface *backgroudFilter;
	menuitem *items;
	unsigned int currentItem;
	SDL_Color colors[2];
	int menuType;
	int returnValue;

	Language lang;

//	string selectedLevel;

	//Level *level; //TODO only testing

	bool running;
	void onExit();
	void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void onMouseMove(int mX, int mY, int xRel, int yRel, bool left, bool right, bool middle);
	void onLButtonDown(int mX,int mY);
	void onWiiButtonEvent(int buttons);

	void render();
	void select(int direction);

	void start();
	void levels();
	void exit();
	void options();
	void quitLevel();
	void back();
	void credits();
	void sound();
	void controllerSettings();
	void continueGame();

public:
	Menu(int menuType=0);
	virtual ~Menu();
	int show();

};



#endif /* MENU_H_ */
