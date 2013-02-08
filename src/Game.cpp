/*
 * game.cpp
 *
 *  Created on: 10.09.2012
 *      Author: philip
 */

#include "Game.h"

Game* Game::curGame;
map<string,Mix_Chunk*> Game::sounds;

Game::Game() {
	curGame = this;
	display = NULL;
	font = NULL;
	currentLevel = NULL;

}

Game::~Game() {
	SDL_FreeSurface(display);
	SDL_Quit();

	//TODO Free all sounds via map iterator

	Mix_CloseAudio();
	Mix_Quit();
}

int Game::execute() {

	init();

	Menu mainMenu;

	return mainMenu.show();
}

void Game::init() {

	/**
	 * Initialize SDL
	 */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
		cout << "unable to initialize SDL" << endl;
	}

	/**
	 * Create Game Window with defined width and height and Bits per pixel use by the system
	 */
	if ((display = SDL_SetVideoMode(WIDTH, HEIGHT,
			SDL_GetVideoInfo()->vfmt->BitsPerPixel,
			SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
		cout << "unable to initialize display" << endl;
	}

	/**
	 * Set Title
	 */
	SDL_WM_SetCaption(TITLE, NULL);

	/**
	 * Initialize SDL_ttf for Fonts
	 */
	if (TTF_Init() < 0) {
		cout << "unable to initialize TTF" << endl;
	}

	/**
	 * Load the menu font
	 */
	if ((font = TTF_OpenFont(FONTS "menu.ttf", 40)) == NULL) {
		cout << "unable to load menufont" << endl;
	}

	/**
	 * Load settings from game.yml file
	 */
	loadSettings();

	/**
	 * Initialize SDL_mixer for audio
	 */
	Mix_Init(MIX_INIT_OGG);

	Mix_OpenAudio(settings.audioRate,AUDIO_S16,2,1024);

	/**
	 * load all the sound files
	 */
	loadSounds();
}

void Game::loadSettings() {
	//TODO if file not exist load default settings and store in file
	YAML::Node settings = YAML::LoadFile(CONFIGS"game.yml");
	this->settings.language = settings["language"].Scalar();
	this->settings.audioRate = settings["audio rate"].as<int>();
}


void Game::loadSounds() {
	sounds["menu"]=Mix_LoadWAV(SOUNDS"menu.ogg");
	sounds["player jump"]=Mix_LoadWAV(SOUNDS"playerJump.ogg");
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
		currentLevel = NULL;
	}
}


TTF_Font* Game::getFont() {
	return font;
}

