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
#include <SDL/SDL_mixer.h>
#include <string>
#include <yaml-cpp/yaml.h>
#include <map>

#include "Menu.h"
#include "Level.h"


using namespace std;

typedef struct{
	string language;
	int audioRate;
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

public:
	settings_t settings;

	Game();
	virtual ~Game();

	int execute();
	void init();

	void loadSounds();

	Level* getCurrentLevel();
	void destroyCurrentLevel();
	void setCurrentLevel(Level *curLev);
	TTF_Font* getFont();
};


#endif /* GAME_H_ */
