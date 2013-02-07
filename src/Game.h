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
#include <string>
#include <yaml-cpp/yaml.h>

#include "Menu.h"
#include "Level.h"


using namespace std;

typedef struct{
	string language;
}settings_t;

class Game {

public:
	static Game *curGame;
	settings_t settings;
private:

	SDL_Surface * display;
	TTF_Font *font;

	Level *currentLevel;

	void loadSettings();

public:
	Game();
	virtual ~Game();

	int execute();
	void init();

	Level* getCurrentLevel();
	void destroyCurrentLevel();
	void setCurrentLevel(Level *curLev);
	TTF_Font* getFont();
};


#endif /* GAME_H_ */
