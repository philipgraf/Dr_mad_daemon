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
	curGame = this;
	display = NULL;
	font = NULL;
	currentLevel = NULL;
}

Game::~Game() {
}

int Game::execute() {
	init();

	Menu();

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

		if(SDL_GetTicks() - fpstime > 1000) {
			char buffer[10];
			sprintf(buffer,"FPS: %d",fps);
			SDL_WM_SetCaption(buffer,NULL);
			fps=0;
			fpstime = SDL_GetTicks();
		}
#endif

	}

	cleanUp();
	return 0;
}

void Game::init() {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		Tools::error("unable to initialize SDL");
	}

	if ((display = SDL_SetVideoMode(WIDTH, HEIGHT, SDL_GetVideoInfo()->vfmt->BitsPerPixel,
			SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
		Tools::error("unable to initialize display");
	}
	SDL_WM_SetCaption(TITLE, NULL);
	if (TTF_Init() < 0) {
		Tools::error("unable to initialize TTF");
	}

	if ((font = TTF_OpenFont(FONTS "menu.ttf", 50)) == NULL) {
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
		Menu();
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

/********************************** GETTER AND SETTER **********************************************************/

Level* Game::getCurrentLevel() {
	return currentLevel;
}

void Game::setCurrentLevel(Level *curLev) {
	currentLevel = curLev;
}

void Game::setRunning(bool r) {
	running = r;
}

bool Game::isRunning() {
	return running;
}

TTF_Font* Game::getFont() {
	return font;
}

