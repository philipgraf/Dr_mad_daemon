#ifndef MENU_H_
#define MENU_H_

#define SIZE(x) (sizeof(x)/sizeof(x[0]))

#define MAINMENU 0
#define SLOTMENU 1
#define OPTIONMENU 2
#define PAUSEMENU 3
#define CREDITS 4
#define LEVELMENU 5
#define GAMEOVER 6

#include <vector>
#include <string>
#include <SDL/SDL.h>

#include "Language.h"
#include "Event.h"

typedef struct {
	SDL_Surface *labelSurface;
	SDL_Rect position;
} menuitem;

/** Menu class.
 * Creates the menus and execute the different actions
 * @author Philip Graf
 * @date 14.03.2013
 * @version 0.1.0 Alpha-State
 */
class Menu: public Event {
	typedef void (Menu::*fptr)();
private:
	std::vector<fptr> labelactions; /**< list of all labelactions */
	std::vector<std::string> labeltexts; /**< list of all labeltexts */
	std::vector<int> labelfonts; /**< list of the font sizes */
	SDL_Surface *background; /**< the background images */
	SDL_Surface *backgroudFilter; /**< a alpha screen for the background */
	menuitem *items; /**< list of all items  */
	unsigned int currentItem; /**< the index of the current selected menuitem */
	SDL_Color colors[2]; /**< 2 colors first for unselected and the second for selected items */
	int menuType; /**< the menutype this is used to built the lists */
	int returnValue; /**< this value will be returned if the menu will be destroyed  */

	Language lang; /**< this objekt is used for translation*/

	bool running; /**< the menu is visible while this is True */
	void onExit();
	void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void onMouseMove(int mX, int mY, int xRel, int yRel, bool left, bool right, bool middle);
	void onLButtonDown(int mX, int mY);
	void onWiiButtonEvent(int buttons);

	void render();
	void select(int direction);
	void build();

	void start();
	void levels();
	void slots();
	void exit();
	void options();
	void quitLevel();
	void back();
	void credits();
	void sound();
	void controllerSettings();
	void continueGame();
	void changeLanguage();
	void changeSlot();
	void createSlot();

public:
	Menu(int menuType = 0);
	virtual ~Menu();
	int show();

};

#endif /* MENU_H_ */
