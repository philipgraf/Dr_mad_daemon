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

	while (running) {
		while (SDL_PollEvent(&event)) {
			onEvent(&event);
		}
		logic();
		render();
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

	return true;
}

void Game::onEvent(SDL_Event* event) {
	Event::onEvent(event);
}

void Game::logic() {
}

void Game::render() {
}

void Game::cleanUp() {
}

void Game::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch (sym) {
	case SDLK_ESCAPE:
		onExit();
		break;
	default:
		break;
	}
}

void Game::onExit() {
	running = false;
}
