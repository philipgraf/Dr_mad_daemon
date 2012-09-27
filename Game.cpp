/*
 * game.cpp
 *
 *  Created on: 10.09.2012
 *      Author: philip
 */

#include "Game.h"

Game::Game() {
	running = true;
	display = NULL;

}
Game::~Game() {
}

int Game::execute() {
	if (!init())
		return -1;

	SDL_Event event;
	Uint32 start;

	while (running) {
		start = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			onEvent(&event);
		}
		logic();
		render();

		if (SDL_GetTicks() - start < 1000 / FPS) {
			SDL_Delay(1000 / FPS - (SDL_GetTicks() - start));
		}
	}

	cleanUp();
	return 0;
}

bool Game::init() {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return false;
	}

	if ((display = SDL_SetVideoMode(WIDTH, HEIGHT, 32,
			SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
		return false;
	}
	background = Tools::loadImage("img/bg.png");
	player = new Entity("img/player.png", 20, HEIGHT - 64 - 20);

	return true;
}

void Game::onEvent(SDL_Event* event) {
	Event::onEvent(event);
}

void Game::logic() {
	player->move();
	cout << player->getX()<< endl;;
}

void Game::render() {
	Tools::drawImage(display, 0, 0, background, 0, 0, display->clip_rect.w,
			display->clip_rect.h);
	player->render();
	SDL_Flip(display);
}

void Game::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch (sym) {
	case SDLK_ESCAPE:
		onExit();
		break;
	case SDLK_LEFT:
		player->setDirection(LEFT);
		break;
	case SDLK_UP:
		player->setDirection(UP);
		break;
	case SDLK_DOWN:
		player->setDirection(DOWN);
		break;
	case SDLK_RIGHT:
		player->setDirection(RIGHT);
		break;
	default:
		break;
	}
}

void Game::onKeyUP(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch (sym) {
	case SDLK_LEFT:
		player->delDirection(LEFT);
		break;
	case SDLK_UP:
		player->delDirection(UP);
		break;
	case SDLK_DOWN:
		player->delDirection(DOWN);
		break;
	case SDLK_RIGHT:
		player->delDirection(RIGHT);
		break;
	default:
		break;
	}
}

void Game::cleanUp() {
	SDL_FreeSurface(display);
	SDL_Quit();
}

void Game::onExit() {
	running = false;
}
