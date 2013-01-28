/*
 * Menu.h
 *
 *  Created on: 28.01.2013
 *      Author: philip
 */

#ifndef MENU_H_
#define MENU_H_

#define SIZE(x) (sizeof(x)/sizeof(x[0]))

#include <iostream>

#include "Event.h"

using namespace std;

class Menu: public Event {
	typedef void (Menu::*fptr)();
private:
	vector<fptr> labelactions;

public:
	Menu();
	virtual ~Menu();

	void mStart() {
		Game::curGame->setRunning(true);
	}
	void mExit() {
		Game::curGame->setRunning(false);
	}
	void mAudio() {
		cout << "Audio an/aus!" << endl;
	}

};

//typedef void (Game::*fptr)();
typedef struct {
	string labelText;
	SDL_Surface *labelSurface;
	SDL_Rect position;
	bool selected;
	//fptr action;
} menuitem;

#endif /* MENU_H_ */
