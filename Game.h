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
#include "Tools.h"
#include "Entity.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <iostream>

#define SIZE(x) (sizeof(x)/sizeof(x[0]))


using namespace std;

class Game: public Event {
	typedef void (Game::*fptr)();
private:
	bool running;
	SDL_Surface * display;
	SDL_Surface * background;
	SDL_Rect view;
	TTF_Font *menufont;
	Entity *player;
	vector<fptr> labelactions;

public:
	Game();
	virtual ~Game();

	int execute();
	void init();
	void menu();
	void onEvent(SDL_Event *event);
	void logic();
	void render();
	void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void onKeyUP(SDLKey sym, SDLMod mod, Uint16 unicode);
	void cleanUp();

	void mStart(){
		running=true;
	}
	void mExit(){
		running=false;
	}
	void mAudio(){
		cout << "Audio an/aus!"<< endl;
	}

	void onExit();


};
//typedef void (Game::*fptr)();
typedef struct{
	string labelText;
	SDL_Surface *labelSurface;
	SDL_Rect position;
	bool selected;
	//fptr action;
}menuitem;

#endif /* GAME_H_ */
