/*
 * Menu.cpp
 *
 *  Created on: 28.01.2013
 *      Author: philip
 */

#include "Menu.h"

Menu::Menu(int menuType) {

	backgroudFilter = NULL;
	background=NULL;
	items=NULL;
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

	switch (menuType) {

	case MAINMENU:
		labeltexts.push_back("Start");
		labelactions.push_back(&Menu::start);
		labeltexts.push_back("Options");
		labelactions.push_back(&Menu::options);
		labeltexts.push_back("Credits");
		labelactions.push_back(&Menu::credits);
		labeltexts.push_back("Exit");
		labelactions.push_back(&Menu::exit);
		break;
	case PAUSEMENU:
		labeltexts.push_back("Continue");
		labelactions.push_back(&Menu::start);
		labeltexts.push_back("Sound");
		labelactions.push_back(&Menu::sound);
		labeltexts.push_back("Quit Level");
		labelactions.push_back(&Menu::quitLevel);
		labeltexts.push_back("Quit Game");
		labelactions.push_back(&Menu::exit);
		break;
	case OPTIONMENU:
		labeltexts.push_back("Sound");
		labelactions.push_back(&Menu::sound);
		labeltexts.push_back("Controller Settings");
		labelactions.push_back(&Menu::controllerSettings);
		labeltexts.push_back("Back");
		labelactions.push_back(&Menu::back);
		break;
	case CREDITS:
		//TODO find better "visualization" ;)
		labeltexts.push_back("Philip Graf");
		labelactions.push_back(&Menu::back);
		labeltexts.push_back("Felix Eckner");
		labelactions.push_back(&Menu::back);
		break;
	}

	if(menuType != PAUSEMENU){
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

int Menu::show() {
	SDL_Surface *screen = SDL_GetVideoSurface();
	Uint32 start;
	SDL_Event event;

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
		items[i].labelSurface = TTF_RenderText_Solid(Game::curGame->getFont(),
				items[i].labelText.c_str(),
				i == currentItem ? colors[1] : colors[0]);
		items[i].position.x = screen->clip_rect.w / 2
				- items[i].labelSurface->clip_rect.w / 2;
		items[i].position.y = (i + 1) * items[i].labelSurface->clip_rect.h;
		items[i].selected = false;
	}

	while (running) {
		start = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			onEvent(&event);
		}

		render();

		SDL_Flip(screen);
		if (SDL_GetTicks() - start < 1000 / FPS) {
			SDL_Delay(1000 / FPS - (SDL_GetTicks() - start));
		}
	}
	return returnValue;
}

/*************************************** MENU ACTIONS **************************************/

void Menu::start() {
	Game::curGame->setRunning(true);
	running = false;
}

void Menu::sound() {
	cout << "Audio an/aus!" << endl;
}

void Menu::options() {
	Menu optionMenu(OPTIONMENU);
	optionMenu.show();
}

void Menu::exit() {
	//TODO if level has the main gameloop then return -1 oder so
	Game::curGame->setRunning(false);
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
	Game::curGame->setRunning(false);
	running = false;
}

void Menu::controllerSettings() {
}

/*************************************** EVENT HANDLING ****************************************/

void Menu::onExit() {
	running = false;
	Game::curGame->setRunning(false);
}

void Menu::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	TTF_Font *menufont = Game::curGame->getFont();
	switch (sym) {
	case SDLK_ESCAPE:
		running = false;
		break;
	case SDLK_UP:
		SDL_FreeSurface(items[currentItem].labelSurface);
		items[currentItem].labelSurface = TTF_RenderText_Solid(menufont,
				items[currentItem].labelText.c_str(), colors[0]);
		currentItem = (currentItem <= 0) ? 0 : (currentItem - 1);
		items[currentItem].labelSurface = TTF_RenderText_Solid(menufont,
				items[currentItem].labelText.c_str(), colors[1]);
		break;
	case SDLK_DOWN:
		SDL_FreeSurface(items[currentItem].labelSurface);
		items[currentItem].labelSurface = TTF_RenderText_Solid(menufont,
				items[currentItem].labelText.c_str(), colors[0]);
		currentItem =
				(currentItem >= labeltexts.size() - 1) ?
						labeltexts.size() - 1 : currentItem + 1;
		items[currentItem].labelSurface = TTF_RenderText_Solid(menufont,
				items[currentItem].labelText.c_str(), colors[1]);
		break;
	case SDLK_RETURN:
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
			items[currentItem].labelSurface = TTF_RenderText_Solid(menufont,
					items[currentItem].labelText.c_str(), colors[0]);
			currentItem = i;
			items[currentItem].labelSurface = TTF_RenderText_Solid(menufont,
					items[currentItem].labelText.c_str(), colors[1]);
		}
	}
}

void Menu::onLButtonDown(int mX, int mY) {
	(this->*labelactions[currentItem])();
	running = false;
}
