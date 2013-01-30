/*
 * Menu.cpp
 *
 *  Created on: 28.01.2013
 *      Author: philip
 */

#include "Menu.h"

Menu::Menu() {

	SDL_Surface *screen = SDL_GetVideoSurface();

	SDL_Event event;
	SDL_Surface *menubackground = SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w,
			screen->h, 32, screen->format->Rmask, screen->format->Gmask,
			screen->format->Bmask, screen->format->Amask);

	running = true;

	Uint32 start;

	SDL_FillRect(menubackground, &screen->clip_rect,
			SDL_MapRGB(screen->format, 0, 0, 0));

	SDL_SetAlpha(menubackground, SDL_SRCALPHA, 128);
	SDL_BlitSurface(menubackground, NULL, screen, NULL);
	SDL_FreeSurface(menubackground);

	if (Game::curGame->isRunning()) {
		labeltexts.push_back("Continue");
		labelactions.push_back(&Menu::mStart);

	} else {
		labeltexts.push_back("Start");
		labelactions.push_back(&Menu::mStart);
	}
	labeltexts.push_back("Audio");
	labelactions.push_back(&Menu::mAudio);
	labeltexts.push_back("Exit");
	labelactions.push_back(&Menu::mExit);



	for (unsigned int i = 0; i < SIZE(items); i++) {
		items[i].labelText = labeltexts[i];
		items[i].labelSurface = TTF_RenderText_Solid(Game::curGame->getFont(),
				items[i].labelText.c_str(),
				i == currentItem ? colors[1] : colors[0]);
		items[i].position.x = screen->clip_rect.w / 2
				- items[i].labelSurface->clip_rect.w / 2;
		items[i].position.y = (i + 1) * items[i].labelSurface->clip_rect.h;
		items[i].selected = false;
		//items[i].action = (this->labelactions[i]);
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

	for (unsigned int i = 0; i < SIZE(items); i++) {
		SDL_FreeSurface(items[i].labelSurface);
	}

	labeltexts.clear();
	labelactions.clear();

}

Menu::~Menu() {
	// TODO Auto-generated destructor stub
}

void Menu::mStart() {
	Game::curGame->setRunning(true);
}

void Menu::mExit() {
	Game::curGame->setRunning(false);
}

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
				(currentItem >= SIZE(items) - 1) ?
						SIZE(items) - 1 : currentItem + 1;
		items[currentItem].labelSurface = TTF_RenderText_Solid(menufont,
				items[currentItem].labelText.c_str(), colors[1]);
		break;
	case SDLK_RETURN:
		(this->*labelactions[currentItem])();
		running = false;
		break;
	default:
		break;
	}
}

void Menu::render() {
	for (unsigned int i = 0; i < SIZE(items); i++) {
		SDL_BlitSurface(items[i].labelSurface, NULL, SDL_GetVideoSurface(),
				&(items[i].position));
	}
}

void Menu::mAudio() {
	cout << "Audio an/aus!" << endl;
}
