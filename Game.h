/*
 * game.h
 *
 *  Created on: 10.09.2012
 *      Author: philip
 */

#include "includes.h"

#ifndef GAME_H_
#define GAME_H_



#define SIZE(x) (sizeof(x)/sizeof(x[0]))


using namespace std;

class Game: public Event {
	typedef void (Game::*fptr)();
public:
	static Game *curGame;
private:
	bool running;
	SDL_Surface * display;

	TTF_Font *menufont;
	vector<fptr> labelactions;

	Level *currentLevel; // TODO only for testing

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

	Level* getCurrentLevel(){
		return currentLevel;
	}



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
