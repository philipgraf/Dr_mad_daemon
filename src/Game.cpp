/*
 * game.cpp
 *
 *  Created on: 10.09.2012
 *      Author: philip
 */

#include "Game.h"

Game* Game::curGame;

Game::Game() {
	curGame = this;
	display = NULL;
	font = NULL;
	currentLevel = NULL;
}

Game::~Game() {
	SDL_FreeSurface(display);
	SDL_Quit();
}

int Game::execute() {

	init();

	Menu mainMenu;

	return mainMenu.show();
}

void Game::init() {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
		cout << "unable to initialize SDL" << endl;
	}

	if ((display = SDL_SetVideoMode(WIDTH, HEIGHT,
			SDL_GetVideoInfo()->vfmt->BitsPerPixel,
			SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
		cout << "unable to initialize display" << endl;
	}
	SDL_WM_SetCaption(TITLE, NULL);
	if (TTF_Init() < 0) {
		cout << "unable to initialize TTF" << endl;
	}

	if ((font = TTF_OpenFont(FONTS "menu.ttf", 40)) == NULL) {
		cout << "unable to load menufont" << endl;
	}

	loadSettings();
}

void Game::loadSettings() {
	//TODO if file not exist load default settings and store in file
	YAML::Node settings = YAML::LoadFile(CONFIGS"game.yml");
	this->settings.language = settings["language"].Scalar();
}

/********************************** GETTER AND SETTER **********************************************************/

Level* Game::getCurrentLevel() {
	return currentLevel;
}

void Game::setCurrentLevel(Level *curLev) {
	currentLevel = curLev;
}

void Game::destroyCurrentLevel() {
	if (currentLevel != NULL) {
		//delete currentLevel;
		currentLevel = NULL;
	}
}

TTF_Font* Game::getFont() {
	return font;
}

