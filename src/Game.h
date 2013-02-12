/*
 * game.h
 *
 *  Created on: 10.09.2012
 *      Author: philip
 */

#ifndef GAME_H_
#define GAME_H_

#include <string>
#include <map>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

#include "Menu.h"
#include "Level.h"

using namespace std;

typedef struct{
	string language;
	int audioRate;
	int activeSlot;
}settings_t;

class Game {

public:
	static Game *curGame;
	static map<string,Mix_Chunk*> sounds;

private:

	SDL_Surface * display;
	TTF_Font *font;

	Level *currentLevel;

	void loadSettings();
	void loadSounds();

public:
	settings_t settings;

	Game();
	virtual ~Game();

	int execute();
	void init();

	void saveSettings();



	Level* getCurrentLevel();
	void destroyCurrentLevel();
	void setCurrentLevel(Level *curLev);
	TTF_Font* getFont();
};


#endif /* GAME_H_ */
