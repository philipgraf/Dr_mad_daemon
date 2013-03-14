/*
 * game.h
 *
 *  Created on: 10.09.2012
 *      Author: philip
 */

#ifndef GAME_H_
#define GAME_H_

#define FONT_MENU_ITEM 0
#define FONT_MENU_HEADER 1
#define FONT_NOTIFICATION 2
#define FONT_PDA_CLOCK 3
#define FONT_PDA_CLOCK_SMALL 4
#define FONT_PDA_CLOCK_TIMER 5

#include <string>
#include <map>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

#include "Menu.h"
#include "Level.h"

typedef struct {
	int left;
	int right;
	int up;
	int down;
	int jump;
	int run;
	int use;
	int grab;
	int pda;
} controll_t;

typedef struct {
	string language;
	int audioRate;
	Uint8 volume;
	int activeSlot;
	controll_t keyboard;
	controll_t wiimote;
} settings_t;

class Game {

public:
	static Game *curGame;
	static map<std::string, Mix_Chunk*> sounds;

private:

	SDL_Surface * display;
	TTF_Font *font[6];
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
	void setCurrentLevel(Level *curLev);
	TTF_Font* getFont(int which = 1);
};

#endif /* GAME_H_ */
