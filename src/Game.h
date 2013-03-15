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
} controll_t;/**< A struct with controlsettings*/

typedef struct {
	string language;
	int audioRate;
	Uint8 volume;
	int activeSlot;
	controll_t keyboard;
	controll_t wiimote;
} settings_t; /**< A struct with gamesettings*/

/** Game class.
 * This class handles the Game initialization and settings
 * @author Philip Graf
 * @date 14.03.2013
 * @version 0.1.0 Alpha-State
 */
class Game {

public:
	static Game *curGame; /**< Pointer to running Game*/
	static map<std::string, Mix_Chunk*> sounds; /**< map with all needed Sound effects*/

private:

	SDL_Surface * display; /**< the whole screen of the game*/
	TTF_Font *font[6]; /**< An Array with different fonts*/
	Level *currentLevel; /**< Pointer to the current level*/

	void loadSettings();
	void loadSounds();

public:
	settings_t settings; /**< A struct with gamesettings*/

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
