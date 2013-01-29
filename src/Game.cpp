/*
 * game.cpp
 *
 *  Created on: 10.09.2012
 *      Author: philip
 */

#include "Game.h"

Game* Game::curGame;

Game::Game() {
	running = false;
	curGame=this;
	display = NULL;
	menufont = NULL;
	currentLevel=NULL;
}
Game::~Game() {
}

int Game::execute() {
	init();

	menu();

	SDL_Event event;
	Uint32 start;
#ifdef DEBUG
	int fps=0;
	int fpstime=0;
#endif
	while (running) {
#ifdef DEBUG
		fps++;
#endif
		start = SDL_GetTicks();


		while (SDL_PollEvent(&event)) {
			onEvent(&event);
		}
		logic();
		render();

		#ifndef DEBUG
		if (SDL_GetTicks() - start < 1000 / FPS) {

			SDL_Delay(1000 / FPS - (SDL_GetTicks() - start));
		}
		#else

		if(SDL_GetTicks() - fpstime > 1000){
			char buffer[10];
			sprintf(buffer,"FPS: %d",fps);
			SDL_WM_SetCaption(buffer,NULL);
			cout << buffer << endl;
			fps=0;
			fpstime = SDL_GetTicks();
		}
		#endif


	}

	cleanUp();
	return 0;
}

void Game::init() {

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0) {
		Tools::error("unable to initialize SDL");
	}

	if ((display = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE )) == NULL) {
		Tools::error("unable to initialize display");
	}
	SDL_WM_SetCaption(TITLE,NULL);
	if (TTF_Init() < 0) {
		Tools::error("unable to initialize TTF");
	}

	if ((menufont = TTF_OpenFont(FONTS "menu.ttf", 50)) == NULL) {
		Tools::error("unable to load menufont");
	}
	Tile::loadTileset();

	currentLevel = new Level();
}

void Game::onEvent(SDL_Event* event) {
	Event::onEvent(event);
}

void Game::logic() {
	currentLevel->logic();
}

void Game::render() {
	currentLevel->render();
	SDL_Flip(display);
}

void Game::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch (sym) {
	case SDLK_ESCAPE:
		menu();
		break;
	case SDLK_LEFT:
		currentLevel->getPlayer()->setDirection(LEFT);
		break;
	case SDLK_UP:
		currentLevel->getPlayer()->setDirection(UP);
		break;
	case SDLK_DOWN:
		currentLevel->getPlayer()->setDirection(DOWN);
		break;
	case SDLK_RIGHT:
		currentLevel->getPlayer()->setDirection(RIGHT);
		break;
	default:
		break;
	}
}

void Game::onKeyUP(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch (sym) {
	case SDLK_LEFT:
		currentLevel->getPlayer()->delDirection(LEFT);
		break;
	case SDLK_UP:
		currentLevel->getPlayer()->delDirection(UP);
		break;
	case SDLK_DOWN:
		currentLevel->getPlayer()->delDirection(DOWN);
		break;
	case SDLK_RIGHT:
		currentLevel->getPlayer()->delDirection(RIGHT);
		break;
	default:
		break;
	}
}

void Game::cleanUp() {
	Entity::entityList.clear();
	SDL_FreeSurface(display);
	SDL_Quit();
}


void Game::onExit() {
	running = false;
}

void Game::menu() {

	vector<string> labeltexts;
	SDL_Surface *screen = SDL_GetVideoSurface();
	SDL_Color colors[2] = { { 255, 255, 255 }, { 91, 176, 248 } };
	SDL_Event event;
	bool menu = true;
	unsigned int currentItem = 0;
	SDL_Surface *menubackground = SDL_CreateRGBSurface(SDL_SWSURFACE,screen->w,screen->h,32,screen->format->Rmask,screen->format->Gmask,screen->format->Bmask,screen->format->Amask);
	Uint32 start;



	SDL_FillRect(menubackground, &screen->clip_rect,
			SDL_MapRGB(screen->format, 0, 0, 0));

	SDL_SetAlpha(menubackground, SDL_SRCALPHA, 128);
	SDL_BlitSurface(menubackground, NULL, screen, NULL);
	SDL_FreeSurface(menubackground);

	if (running) {
		labeltexts.push_back("Continue");
		labelactions.push_back(&Game::mStart);

	} else {
		labeltexts.push_back("Start");
		labelactions.push_back(&Game::mStart);
	}
	labeltexts.push_back("Audio");
	labelactions.push_back(&Game::mAudio);
	labeltexts.push_back("Exit");
	labelactions.push_back(&Game::mExit);

	menuitem items[labeltexts.size()];

	for (unsigned int i = 0; i < SIZE(items); i++) {
		items[i].labelText = labeltexts[i];
		items[i].labelSurface = TTF_RenderText_Solid(menufont,
				items[i].labelText.c_str(),
				i == currentItem ? colors[1] : colors[0]);
		items[i].position.x = screen->clip_rect.w / 2
				- items[i].labelSurface->clip_rect.w / 2;
		items[i].position.y = (i + 1) * items[i].labelSurface->clip_rect.h;
		items[i].selected = false;
		//items[i].action = (this->labelactions[i]);
	}

	while (menu) {
		start = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				menu = false;
				running = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					menu = false;
					break;
				case SDLK_UP:
					SDL_FreeSurface(items[currentItem].labelSurface);
					items[currentItem].labelSurface = TTF_RenderText_Solid(
							menufont, items[currentItem].labelText.c_str(),
							colors[0]);
					currentItem = (currentItem <= 0) ? 0 : (currentItem - 1);
					items[currentItem].labelSurface = TTF_RenderText_Solid(
							menufont, items[currentItem].labelText.c_str(),
							colors[1]);
					break;
				case SDLK_DOWN:
					SDL_FreeSurface(items[currentItem].labelSurface);
					items[currentItem].labelSurface = TTF_RenderText_Solid(
							menufont, items[currentItem].labelText.c_str(),
							colors[0]);
					currentItem =
							(currentItem >= SIZE(items) - 1) ?
									SIZE(items) - 1 : currentItem + 1;
					items[currentItem].labelSurface = TTF_RenderText_Solid(
							menufont, items[currentItem].labelText.c_str(),
							colors[1]);
					break;
				case SDLK_RETURN:
					(this->*labelactions[currentItem])();
					menu = false;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}

		for (unsigned int i = 0; i < SIZE(items); i++) {
			SDL_BlitSurface(items[i].labelSurface, NULL, screen,
					&(items[i].position));
		}

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

