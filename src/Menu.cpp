#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <sstream>

#include "Menu.h"
#include "define.h"
#include "Game.h"
#include "Level.h"
#include "TextInput.h"
#include "Slot.h"

/**
 * set the type of the menu and call the build function
 * @param menuType the menu type
 * @see build()
 */
Menu::Menu(int menuType) {

	backgroudFilter = NULL;
	background = NULL;
	items = NULL;
	running = true;
	this->menuType = menuType;
	returnValue = 0;

	currentItem = 0;
	colors[0].r = 255;
	colors[0].g = 255;
	colors[0].b = 255;
	colors[1].r = 91;
	colors[1].g = 176;
	colors[1].b = 248;

	build();

	if (menuType != PAUSEMENU && menuType != GAMEOVER ) {
		SDL_Surface *tmp = SDL_LoadBMP(IMG"menubg.bmp");
		background = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
	}

}

/**
 * free all the allocated memory
 */
Menu::~Menu() {
	for (unsigned int i = 0; i < labeltexts.size(); i++) {
		SDL_FreeSurface(items[i].labelSurface);
	}

	SDL_FreeSurface(backgroudFilter);
	labeltexts.clear();
	labelactions.clear();
}

/**
 * build all the labels based on the menu type
 */
void Menu::build() {
	std::stringstream vol;
	vol << Game::curGame->settings.volume/128.0*100 << "%";

	SDL_Surface *screen = SDL_GetVideoSurface();
	for (unsigned int i = 0; i < labeltexts.size(); i++) {
		delete items[i].labelSurface;
	}

	labelactions.clear();
	labeltexts.clear();
	labelfonts.clear();
	switch (menuType) {

	case MAINMENU:
		labeltexts.push_back(lang["play"]);
		labelfonts.push_back(FONT_MENU_HEADER);
		labelactions.push_back(&Menu::levels);
		labeltexts.push_back(lang["options"]);
		labelfonts.push_back(FONT_MENU_HEADER);
		labelactions.push_back(&Menu::options);
		labeltexts.push_back(lang["credits"]);
		labelfonts.push_back(FONT_MENU_HEADER);
		labelactions.push_back(&Menu::credits);
		labeltexts.push_back(lang["exit"]);
		labelfonts.push_back(FONT_MENU_HEADER);
		labelactions.push_back(&Menu::exit);
		break;
	case PAUSEMENU:
		labeltexts.push_back(lang["continue"]);
		labelfonts.push_back(FONT_MENU_HEADER);
		labelactions.push_back(&Menu::continueGame);
		labeltexts.push_back(lang["sound"]);
		labelfonts.push_back(FONT_MENU_HEADER);
		labelactions.push_back(&Menu::sound);
		labeltexts.push_back(lang["quit level"]);
		labelfonts.push_back(FONT_MENU_HEADER);
		labelactions.push_back(&Menu::quitLevel);
		labeltexts.push_back(lang["quit game"]);
		labelfonts.push_back(FONT_MENU_HEADER);
		labelactions.push_back(&Menu::exit);
		break;
	case OPTIONMENU:

		labeltexts.push_back(lang["sound"] + ": " + vol.str());
		labelfonts.push_back(FONT_MENU_ITEM);
		labelactions.push_back(&Menu::sound);
		labeltexts.push_back(lang["language"] + ": " + Game::curGame->settings.language);
		labelfonts.push_back(FONT_MENU_ITEM);
		labelactions.push_back(&Menu::changeLanguage);
		labeltexts.push_back(lang["connect wiimote"]);
		labelfonts.push_back(FONT_MENU_ITEM);
		labelactions.push_back(&Menu::initWiimote);
		labeltexts.push_back(lang["back"]);
		labelfonts.push_back(FONT_MENU_HEADER);
		labelactions.push_back(&Menu::back);
		break;
	case CREDITS:
		//TODO find better "visualization" ;)
		labeltexts.push_back("Philip Graf");
		labelfonts.push_back(FONT_MENU_ITEM);
		labelactions.push_back(&Menu::back);
		labeltexts.push_back("Felix Eckner");
		labelfonts.push_back(FONT_MENU_ITEM);
		labelactions.push_back(&Menu::back);
		break;
	case LEVELMENU:
		labeltexts.push_back("Slot: " + (Slot::slots.size() == 0 ? "None" : Slot::slots[Game::curGame->settings.activeSlot]->getName()));
		labelfonts.push_back(FONT_MENU_HEADER);
		labelactions.push_back(&Menu::slots);
		if (Game::curGame->settings.activeSlot != -1) {
			for (int i = 0; i <= Slot::slots[Game::curGame->settings.activeSlot]->getFinishedLevels(); i++) {
				labeltexts.push_back(Level::levelnames[Level::levels[i]]);
				labelfonts.push_back(FONT_MENU_ITEM);
				labelactions.push_back(&Menu::start);
			}
		}
		labeltexts.push_back(lang["back"]);
		labelfonts.push_back(FONT_MENU_HEADER);
		labelactions.push_back(&Menu::back);
		break;
	case SLOTMENU:
		for (unsigned i = 0; i < Slot::slots.size(); i++) {
			labeltexts.push_back(Slot::slots[i]->getName());
			labelfonts.push_back(FONT_MENU_ITEM);
			labelactions.push_back(&Menu::changeSlot);
		}
		labeltexts.push_back(lang["new slot"]);
		labelfonts.push_back(FONT_MENU_HEADER);
		labelactions.push_back(&Menu::createSlot);
		labeltexts.push_back(lang["back"]);
		labelfonts.push_back(FONT_MENU_HEADER);
		labelactions.push_back(&Menu::back);
		break;
	case GAMEOVER:
		labeltexts.push_back(lang["game over"]);
		labelfonts.push_back(FONT_MENU_HEADER);
		labelactions.push_back(&Menu::quitLevel);
		break;
	}

	backgroudFilter = SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w, screen->h, SDL_GetVideoInfo()->vfmt->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);

	SDL_FillRect(backgroudFilter, &screen->clip_rect, SDL_MapRGB(screen->format, 0, 0, 0));

	SDL_SetAlpha(backgroudFilter, SDL_SRCALPHA, 128);

	SDL_Rect dstRect = { TILESIZE * 3, TILESIZE, backgroudFilter->clip_rect.w, backgroudFilter->clip_rect.h };
	SDL_Rect srcRect;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = screen->clip_rect.w - 6 * TILESIZE;
	srcRect.h = screen->clip_rect.h - 2 * TILESIZE;

	SDL_BlitSurface(backgroudFilter, &srcRect, screen, &dstRect);

	items = new menuitem[labeltexts.size()];
	int positionY = TILESIZE*2;
	for (unsigned int i = 0; i < labeltexts.size(); i++) {
		items[i].labelSurface = TTF_RenderUTF8_Blended(Game::curGame->getFont(labelfonts[i]), labeltexts[i].c_str(), i == currentItem ? colors[1] : colors[0]);
		items[i].position.x = screen->clip_rect.w / 2 - items[i].labelSurface->clip_rect.w / 2;
		items[i].position.y = positionY;
		positionY += items[i].labelSurface->clip_rect.h;
	}

}

/**
 * render the background and the menu label on the screen
 */
void Menu::render() {
	SDL_Surface *screen = SDL_GetVideoSurface();

	if (background != NULL) {
		SDL_BlitSurface(background, NULL, screen, NULL);
		SDL_Rect dstRect = { TILESIZE * 3, TILESIZE, backgroudFilter->clip_rect.w, backgroudFilter->clip_rect.h };
		SDL_Rect srcRect;
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = screen->clip_rect.w - 6 * TILESIZE;
		srcRect.h = screen->clip_rect.h - 2 * TILESIZE;
		SDL_BlitSurface(backgroudFilter, &srcRect, screen, &dstRect);

	}

	for (unsigned int i = 0; i < labeltexts.size(); i++) {
		SDL_BlitSurface(items[i].labelSurface, NULL, screen, &(items[i].position));
	}
}

/**
 * play the sound and change the color and the current selected index
 * @param direction the direction that the currentItem index will change (UP or DOWN)
 */
void Menu::select(int direction) {

	Mix_PlayChannel(-1, Game::sounds["menu select"], 0);

	SDL_FreeSurface(items[currentItem].labelSurface);
	items[currentItem].labelSurface = TTF_RenderUTF8_Blended(Game::curGame->getFont(labelfonts[currentItem]), labeltexts[currentItem].c_str(), colors[0]);

	//item below
	if (direction & DOWN) {
		currentItem = (currentItem >= labeltexts.size() - 1) ? labeltexts.size() - 1 : currentItem + 1;
		//item above
	} else if (direction & UP) {
		currentItem = (currentItem <= 0) ? 0 : (currentItem - 1);
	}

	items[currentItem].labelSurface = TTF_RenderUTF8_Blended(Game::curGame->getFont(labelfonts[currentItem]), labeltexts[currentItem].c_str(), colors[1]);
}

/**
 * the menu loop. Handles the Framecontrol and the user inputs.
 * This loop runs until running is false.
 */
int Menu::show() {
	Uint32 start;
	SDL_Event event;

	while (running) {
		start = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			onEvent(&event);
		}
		render();

		SDL_Flip(SDL_GetVideoSurface());
		if (SDL_GetTicks() - start < 1000 / FPS) {
			SDL_Delay(1000 / FPS - (SDL_GetTicks() - start));
		}
	}
	return returnValue;
}

/*************************************** MENU ACTIONS **************************************/
/**
 * start the selected level
 */
void Menu::start() {

	Level level((unsigned) currentItem - 1);
	level.play();
	build();
}

/**
 * change the language and rebuild the menu with the new language
 */
void Menu::changeLanguage() {
	int index = -1;
	for (unsigned i = 0; i < Language::supLanguages.size(); i++) {
		if (Language::supLanguages[i] == Game::curGame->settings.language) {
			index = i;
		}
	}

	if (index != -1) {
		index++;
		if (index >= (int) Language::supLanguages.size()) {
			index = 0;
		}
		Game::curGame->settings.language = Language::supLanguages[index];
	} else {

		Game::curGame->settings.language = Language::supLanguages[0];
	}
	build();
}

/**
 * change the active slot to the currently selected one.
 */
void Menu::changeSlot() {

	Game::curGame->settings.activeSlot = currentItem;
	Game::curGame->saveSettings();
	running = false;
}

/**
 * create a textinput field and create an new slot with the return name.
 * @see TextInput
 */
void Menu::createSlot() {
	string slotname = TextInput("Enter slotname:", 10).getInput();
	if (slotname != "") {
		Slot::slots.push_back(new Slot(slotname));
		Game::curGame->settings.activeSlot = Slot::slots.size() - 1;
		Slot::saveSlots();
		Game::curGame->saveSettings();
	}
	running = false;
}

/**
 * create a new menu with the menu type LEVELMENU
 */
void Menu::levels() {
	Menu levelmenu(LEVELMENU);
	levelmenu.show();
}

/**
 * create a new menu with the menu type SLOTMENU
 */
void Menu::slots() {
	Menu slotmenu(SLOTMENU);
	slotmenu.show();
	build();
}

/**
 * set running of the PAUSEMENU to false.
 */
void Menu::continueGame() {
	this->running = false;
}

/**
 * change the volume in 25 per cent steps.
 * the volume of the music always have half of the sounds
 */
void Menu::sound() {
	Game::curGame->settings.volume=(Game::curGame->settings.volume+32)%(MIX_MAX_VOLUME+32);
	Mix_VolumeMusic(Game::curGame->settings.volume/2);
	Mix_Volume(-1,Game::curGame->settings.volume);
	// TODO make current volume visible in Pausemenu
	if (menuType != PAUSEMENU) {
		build();
	}
}

/**
 * create a new menu with the menu type OPTIONSMENU
 */
void Menu::options() {
	Menu optionMenu(OPTIONMENU);
	optionMenu.show();
	build();
	Game::curGame->saveSettings();
}

/**
 * set running to false and quit the whole game
 */
void Menu::exit() {
	//TODO if level has the main gameloop then return -1. set returnvalue to pass throw and quit the whole game

	//Game::curGame->getCurrentLevel()->setRunning(false);
	running = false;
	returnValue = -1;
}
/**
 * create a new menu with the menu type CREDITS
 */
void Menu::credits() {
	Menu creditsMenu(CREDITS);
	creditsMenu.show();
}

/**
 * get back to the menu before
 */
void Menu::back() {
	running = false;
}

/**
 * quit the current level
 */
void Menu::quitLevel() {
	Game::curGame->getCurrentLevel()->setRunning(false);
	//	delete level;
	//	level=NULL;
	running = false;
}

/**
 * not implemented yet.
 */
void Menu::controllerSettings() {
}

/*************************************** EVENT HANDLING ****************************************/

/**
 * the event which will be called if the close button is clicked.
 * quit the game.
 */
void Menu::onExit() {
	exit();
}

/**
 * handle the key events
 * @param sym the symbol of the key
 * @param mod modifications like left-Shift or alt button
 * @param unicode the unicode value of the pressed key
 */
void Menu::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch (sym) {
	case SDLK_ESCAPE:
		running = false;
		break;
	case SDLK_UP:
		select(UP);
		break;
	case SDLK_DOWN:
		select(DOWN);
		break;
	case SDLK_RETURN:
		Mix_PlayChannel(-1, Game::sounds["menu confirm"], 0);
		(this->*labelactions[currentItem])();
		break;
	default:
		break;
	}
}

/**
 * handle the wii button events
 */
void Menu::onWiiButtonEvent(int buttons) {
	switch (buttons) {
	// if the wiimote is horizontal Left is down
	case WII_BTN_LEFT:
		select(DOWN);
		break;
	case WII_BTN_RIGHT:
		select(UP);
		break;
	case WII_BTN_2:
		(this->*labelactions[currentItem])();
		break;
	case WII_BTN_1:
	case WII_BTN_HOME:
		running=false;
		break;
	default:
		break;

	}
}

/**
 * mouse movement handler
 * set the currentitem to the item under the mouse.
 */
void Menu::onMouseMove(int mX, int mY, int xRel, int yRel, bool left, bool right, bool middle) {
	for (unsigned i = 0; i < labeltexts.size(); i++) {
		if (mX > items[i].position.x && mX < items[i].position.x + items[i].position.w && mY > items[i].position.y && mY < items[i].position.y + items[i].position.h) {
			SDL_FreeSurface(items[currentItem].labelSurface);
			items[currentItem].labelSurface = TTF_RenderUTF8_Blended(Game::curGame->getFont(labelfonts[currentItem]), labeltexts[currentItem].c_str(), colors[0]);
			currentItem = i;
			items[currentItem].labelSurface = TTF_RenderUTF8_Blended(Game::curGame->getFont(labelfonts[currentItem]), labeltexts[currentItem].c_str(), colors[1]);
		}
	}
}

/**
 * mouse button handler
 * execute the labelaction of the current selected menuitem
 */
void Menu::onLButtonDown(int mX, int mY) {
	(this->*labelactions[currentItem])();
}

