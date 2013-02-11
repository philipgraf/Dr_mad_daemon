/*
 * Menu.cpp
 *
 *  Created on: 28.01.2013
 *      Author: philip
 */

#include "Menu.h"

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

	if (menuType != PAUSEMENU) {
		SDL_Surface *tmp = SDL_LoadBMP(IMG"menubg.bmp");
		background = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
	}

}

Menu::~Menu() {
	for (unsigned int i = 0; i < labeltexts.size(); i++) {
		SDL_FreeSurface(items[i].labelSurface);
	}

	SDL_FreeSurface(backgroudFilter);
	labeltexts.clear();
	labelactions.clear();
}

void Menu::build() {

	SDL_Surface *screen = SDL_GetVideoSurface();
	for (unsigned int i = 0; i < labeltexts.size(); i++) {
		delete items[i].labelSurface;
	}

	labelactions.clear();
	labeltexts.clear();
	switch (menuType) {

	case MAINMENU:
		labeltexts.push_back(lang["play"]);
		labelactions.push_back(&Menu::levels);
		labeltexts.push_back(lang["options"]);
		labelactions.push_back(&Menu::options);
		labeltexts.push_back(lang["credits"]);
		labelactions.push_back(&Menu::credits);
		labeltexts.push_back(lang["exit"]);
		labelactions.push_back(&Menu::exit);
		break;
	case PAUSEMENU:
		labeltexts.push_back(lang["continue"]);
		labelactions.push_back(&Menu::continueGame);
		labeltexts.push_back(lang["sound"]);
		labelactions.push_back(&Menu::sound);
		labeltexts.push_back(lang["quit level"]);
		labelactions.push_back(&Menu::quitLevel);
		labeltexts.push_back(lang["quit game"]);
		labelactions.push_back(&Menu::exit);
		break;
	case OPTIONMENU:
		labeltexts.push_back(lang["sound"]);
		labelactions.push_back(&Menu::sound);
		labeltexts.push_back(
				lang["language"] + ":" + Game::curGame->settings.language);
		labelactions.push_back(&Menu::changeLanguage);
		labeltexts.push_back(lang["controller"]);
		labelactions.push_back(&Menu::controllerSettings);
		labeltexts.push_back(lang["back"]);
		labelactions.push_back(&Menu::back);
		break;
	case CREDITS:
		//TODO find better "visualization" ;)
		labeltexts.push_back("Philip Graf");
		labelactions.push_back(&Menu::back);
		labeltexts.push_back("Felix Eckner");
		labelactions.push_back(&Menu::back);
		break;
	case LEVELMENU:
		labeltexts.push_back("Slot: "+ (Slot::slotnames.size() == 0 ? "None" : Slot::slotnames[Game::curGame->settings.activeSlot]));
		labelactions.push_back(&Menu::slots);
		if (Game::curGame->settings.activeSlot != -1) {
			for (int i = 0; i <= Slot::slots[Slot::slotnames[Game::curGame->settings.activeSlot]]->getFinishedLevels(); i++) {
				labeltexts.push_back(Level::levelnames[Level::levels[i]]);
				labelactions.push_back(&Menu::start);
			}
		}
		labeltexts.push_back(lang["back"]);
		labelactions.push_back(&Menu::back);
		break;
	case SLOTMENU:
		for(int i=0;i < Slot::slotnames.size();i++){
			labeltexts.push_back(Slot::slotnames[i]);
			labelactions.push_back(&Menu::changeSlot);
		}
		labeltexts.push_back(lang["new slot"]);
		labelactions.push_back(&Menu::createSlot);
		labeltexts.push_back(lang["back"]);
		labelactions.push_back(&Menu::back);
		break;
	}

	backgroudFilter = SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w, screen->h,
			SDL_GetVideoInfo()->vfmt->BitsPerPixel, screen->format->Rmask,
			screen->format->Gmask, screen->format->Bmask,
			screen->format->Amask);

	SDL_FillRect(backgroudFilter, &screen->clip_rect,
			SDL_MapRGB(screen->format, 0, 0, 0));

	SDL_SetAlpha(backgroudFilter, SDL_SRCALPHA, 128);

	SDL_SetAlpha(backgroudFilter, SDL_SRCALPHA, 128);
	SDL_Rect dstRect = { TILESIZE * 3, TILESIZE, backgroudFilter->clip_rect.w,
			backgroudFilter->clip_rect.h };
	SDL_Rect srcRect = { 0, 0, screen->w - 6 * TILESIZE, screen->h
			- 2 * TILESIZE };
	SDL_BlitSurface(backgroudFilter, &srcRect, screen, &dstRect);

	items = new menuitem[labeltexts.size()];

	for (unsigned int i = 0; i < labeltexts.size(); i++) {
		items[i].labelText = labeltexts[i];
		items[i].labelSurface = TTF_RenderUTF8_Solid(Game::curGame->getFont(),
				items[i].labelText.c_str(),
				i == currentItem ? colors[1] : colors[0]);
		items[i].position.x = screen->clip_rect.w / 2
				- items[i].labelSurface->clip_rect.w / 2;
		items[i].position.y = (i + 1) * items[i].labelSurface->clip_rect.h;
		items[i].selected = false;
	}

}

void Menu::render() {
	SDL_Surface *screen = SDL_GetVideoSurface();

	if (background != NULL) {
		SDL_BlitSurface(background, NULL, screen, NULL);
		SDL_Rect dstRect = { TILESIZE * 3, TILESIZE,
				backgroudFilter->clip_rect.w, backgroudFilter->clip_rect.h };
		SDL_Rect srcRect = { 0, 0, screen->w - 6 * TILESIZE, screen->h
				- 2 * TILESIZE };
		SDL_BlitSurface(backgroudFilter, &srcRect, screen, &dstRect);

	}

	for (unsigned int i = 0; i < labeltexts.size(); i++) {
		SDL_BlitSurface(items[i].labelSurface, NULL, SDL_GetVideoSurface(),
				&(items[i].position));
	}
}

void Menu::select(int direction) {

	Mix_PlayChannel(-1, Game::sounds["menu select"], 0);

	TTF_Font *menufont = Game::curGame->getFont();
	SDL_FreeSurface(items[currentItem].labelSurface);
	items[currentItem].labelSurface = TTF_RenderUTF8_Solid(menufont,
			items[currentItem].labelText.c_str(), colors[0]);

	//item below
	if (direction & DOWN) {
		currentItem =
				(currentItem >= labeltexts.size() - 1) ?
						labeltexts.size() - 1 : currentItem + 1;
		//item above
	} else if (direction & UP) {
		currentItem = (currentItem <= 0) ? 0 : (currentItem - 1);
	}

	items[currentItem].labelSurface = TTF_RenderUTF8_Solid(menufont,
			items[currentItem].labelText.c_str(), colors[1]);
}

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

void Menu::start() {

	Level level(Level::levels[currentItem-1]);
	level.play();
}

void Menu::changeLanguage() {
	int index = -1;
	for (int i = 0; i < Language::supLanguages.size(); i++) {
		if (Language::supLanguages[i] == Game::curGame->settings.language) {
			index = i;
		}
	}

	if (index != -1) {
		index++;
		if (index >= Language::supLanguages.size()) {
			index = 0;
		}
		Game::curGame->settings.language = Language::supLanguages[index];
	} else {

		Game::curGame->settings.language = Language::supLanguages[0];
	}
	build();
}

void Menu::changeSlot(){

	Game::curGame->settings.activeSlot = currentItem;
	running= false;
}

void Menu::createSlot(){
	Slot::slots["TestSlot"] = new Slot("TestSlot");
	Slot::slotnames.push_back("TestSlot");
	Game::curGame->settings.activeSlot= Slot::slotnames.size()-1;
	running=false;
}

void Menu::levels() {
	Menu levelmenu(LEVELMENU);
	levelmenu.show();
}

void Menu::slots(){
	Menu slotmenu(SLOTMENU);
	slotmenu.show();
	build();
}

void Menu::continueGame() {
	this->running = false;
}

void Menu::sound() {
	cout << "Audio an/aus!" << endl;
}

void Menu::options() {
	Menu optionMenu(OPTIONMENU);
	optionMenu.show();
	build();
	Game::curGame->saveSettings();
}

void Menu::exit() {
	//TODO if level has the main gameloop then return -1 oder so
	//	Game::curGame->setRunning(false);
	//TODO set returnvalue to pass throw and quit the whole game

	//Game::curGame->getCurrentLevel()->setRunning(false);
	running = false;
}

void Menu::credits() {
	Menu creditsMenu(CREDITS);
	creditsMenu.show();
}

void Menu::back() {
	running = false;
}

void Menu::quitLevel() {
	Game::curGame->getCurrentLevel()->setRunning(false);
	//	delete level;
	//	level=NULL;
	running = false;
}

void Menu::controllerSettings() {
}

/*************************************** EVENT HANDLING ****************************************/

void Menu::onExit() {
	exit();
}

void Menu::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	TTF_Font *menufont = Game::curGame->getFont();
	switch (sym) {
	case SDLK_TAB:
		initWiimote();
		break;
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

void Menu::onWiiButtonEvent(int buttons) {
	TTF_Font *menufont = Game::curGame->getFont();
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
	default:
		break;

	}
}

void Menu::onMouseMove(int mX, int mY, int xRel, int yRel, bool left,
		bool right, bool middle) {
	TTF_Font *menufont = Game::curGame->getFont();
	for (int i = 0; i < labeltexts.size(); i++) {
		if (mX > items[i].position.x
				&& mX < items[i].position.x + items[i].position.w
				&& mY > items[i].position.y
				&& mY < items[i].position.y + items[i].position.h) {
			SDL_FreeSurface(items[currentItem].labelSurface);
			items[currentItem].labelSurface = TTF_RenderUTF8_Solid(menufont,
					items[currentItem].labelText.c_str(), colors[0]);
			currentItem = i;
			items[currentItem].labelSurface = TTF_RenderUTF8_Solid(menufont,
					items[currentItem].labelText.c_str(), colors[1]);
		}
	}
}

void Menu::onLButtonDown(int mX, int mY) {
	(this->*labelactions[currentItem])();
}

