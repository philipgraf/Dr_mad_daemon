/*
 * game.cpp
 *
 *  Created on: 10.09.2012
 *      Author: philip
 */

#include "Game.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

#include "Slot.h"

using namespace std;

Game* Game::curGame;
map<string, Mix_Chunk*> Game::sounds;

Game::Game() {
	curGame = this;
	display = NULL;
	currentLevel = NULL;

}

Game::~Game() {
	SDL_FreeSurface(display);
	SDL_Quit();

	for(map<string,Mix_Chunk*>::iterator it = sounds.begin(); it != sounds.end();++it){
		Mix_FreeChunk((*it).second);
	}

	sounds.clear();

	Language::supLanguages.clear();
	Level::levels.clear();
	Level::levelnames.clear();

	Mix_CloseAudio();
	Mix_Quit();
}

int Game::execute() {

	init();

	Menu mainMenu;
	int ret = mainMenu.show();
	SDL_FillRect(SDL_GetVideoSurface(), &SDL_GetVideoSurface()->clip_rect, SDL_MapRGB(SDL_GetVideoSurface()->format, 0, 0, 0));
	SDL_Color white;
	white.b = 255;
	white.r = 255;
	white.g = 255;
	SDL_Surface *text = TTF_RenderUTF8_Blended(TTF_OpenFont(FONTS"menu.ttf", 72), "Good Bye!", white);
	SDL_Rect dest;
	dest.x = (display->w - text->w) / 2;
	dest.y = (display->h - text->h) / 2;
	dest.w = text->w;
	dest.h = text->h;
	SDL_BlitSurface(text, NULL, SDL_GetVideoSurface(), &dest);

	SDL_Flip(SDL_GetVideoSurface());
	return ret;
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
	if ((display = SDL_SetVideoMode(WIDTH, HEIGHT, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_SWSURFACE | SDL_FULLSCREEN)) == NULL) {
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
	 * Load the menu Header font
	 */
	if ((font[FONT_MENU_HEADER] = TTF_OpenFont(FONTS "menu.ttf", 40)) == NULL) {
		cout << "unable to load menu header font" << endl;
	}
	if ((font[FONT_MENU_ITEM] = TTF_OpenFont(FONTS "menu.ttf", 30)) == NULL) {
		cout << "unable to load menu item font" << endl;
	}
	if ((font[FONT_NOTIFICATION] = TTF_OpenFont(FONTS "menu.ttf", 16)) == NULL) {
		cout << "unable to load notification Font" << endl;
	}
	if ((font[FONT_PDA_CLOCK] = TTF_OpenFont(FONTS "digital.ttf", 18)) == NULL) {
		cout << "unable to load notification Font" << endl;
	}
	if ((font[FONT_PDA_CLOCK_TIMER] = TTF_OpenFont(FONTS "digital.ttf", 32)) == NULL) {
		cout << "unable to load notification Font" << endl;
	}

	/**
	 * Load settings from game.yml file
	 */
	loadSettings();

	/**
	 * Initialize SDL_mixer for audio
	 */
	Mix_Init(MIX_INIT_OGG);

	Mix_OpenAudio(settings.audioRate, AUDIO_S16, 2, 1024);

	/**
	 * load all the sound files
	 */
	loadSounds();

	Level::loadLevels();

	Slot::loadSlots();

	Language::getSupportedLanguages();
}

void Game::loadSettings() {
	YAML::Node settings;
	try {
		settings = YAML::LoadFile(CONFIGS"game.yml");
	} catch (YAML::Exception &e) {
		cout << e.msg << endl;
	}

	if (settings["language"].Scalar() == YAML::detail::node_data::empty_scalar) {
		this->settings.language = "en";
	} else {
		this->settings.language = settings["language"].Scalar();
	}
	if (settings["audio rate"].Scalar() == YAML::detail::node_data::empty_scalar) {
		this->settings.audioRate = 22050;
	} else {
		this->settings.audioRate = settings["audio rate"].as<int>();
	}

	if (settings["active slot"].Scalar() == YAML::detail::node_data::empty_scalar) {
		this->settings.activeSlot = -1;
	} else {
		this->settings.activeSlot = settings["active slot"].as<int>();
	}

	if (settings["keyboard settings"].Scalar() == YAML::detail::node_data::empty_scalar) {
		this->settings.controller.left = SDLK_a;
		this->settings.controller.right = SDLK_d;
		this->settings.controller.up = SDLK_w;
		this->settings.controller.down = SDLK_s;
		this->settings.controller.run = SDLK_LSHIFT;
		this->settings.controller.jump = SDLK_SPACE;
		this->settings.controller.use = SDLK_e;
	} else {
		if (settings["keyboard settings"]["left"].Scalar() == YAML::detail::node_data::empty_scalar) {
			this->settings.controller.left = SDLK_a;
		} else {
			this->settings.controller.left = settings["keyboard settings"]["left"].as<int>();
		}

		if (settings["keyboard settings"]["right"].Scalar() == YAML::detail::node_data::empty_scalar) {
			this->settings.controller.right = SDLK_d;
		} else {
			this->settings.controller.right = settings["keyboard settings"]["right"].as<int>();
		}

		if (settings["keyboard settings"]["up"].Scalar() == YAML::detail::node_data::empty_scalar) {
			this->settings.controller.up = SDLK_w;
		} else {
			this->settings.controller.up = settings["keyboard settings"]["up"].as<int>();
		}

		if (settings["keyboard settings"]["down"].Scalar() == YAML::detail::node_data::empty_scalar) {
			this->settings.controller.down = SDLK_s;
		} else {
			this->settings.controller.down = settings["keyboard settings"]["down"].as<int>();
		}

		if (settings["keyboard settings"]["run"].Scalar() == YAML::detail::node_data::empty_scalar) {
			this->settings.controller.run = SDLK_LSHIFT;
		} else {
			this->settings.controller.run = settings["keyboard settings"]["run"].as<int>();
		}

		if (settings["keyboard settings"]["jump"].Scalar() == YAML::detail::node_data::empty_scalar) {
			this->settings.controller.jump = SDLK_SPACE;
		} else {
			this->settings.controller.jump = settings["keyboard settings"]["jump"].as<int>();
		}

		if (settings["keyboard settings"]["use"].Scalar() == YAML::detail::node_data::empty_scalar) {
			this->settings.controller.use = SDLK_e;
		} else {
			this->settings.controller.use = settings["keyboard settings"]["use"].as<int>();
		}
	}

}

void Game::loadSounds() {
	sounds["menu select"] = Mix_LoadWAV(SOUNDS"menu_select.ogg");
	sounds["menu confirm"] = Mix_LoadWAV(SOUNDS"menu_confirm.ogg");
	sounds["player jump"] = Mix_LoadWAV(SOUNDS"player_jump.ogg");
	sounds["player jump impact"] = Mix_LoadWAV(SOUNDS"player_jump_impact.ogg");
}

void Game::saveSettings() {

	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "language";
	out << YAML::Value << settings.language;
	out << YAML::Key << "audio rate";
	out << YAML::Value << settings.audioRate;
	out << YAML::Key << "active slot";
	out << YAML::Value << settings.activeSlot;
	out << YAML::Key << "keyboard settings";
	out << YAML::BeginMap;
	out << YAML::Key << "left";
	out << YAML::Value << settings.controller.left;
	out << YAML::Key << "right";
	out << YAML::Value << settings.controller.right;
	out << YAML::Key << "up";
	out << YAML::Value << settings.controller.up;
	out << YAML::Key << "down";
	out << YAML::Value << settings.controller.down;
	out << YAML::Key << "run";
	out << YAML::Value << settings.controller.run;
	out << YAML::Key << "jump";
	out << YAML::Value << settings.controller.jump;
	out << YAML::Key << "use";
	out << YAML::Value << settings.controller.use;
	out << YAML::EndMap;
	out << YAML::EndMap;

	fstream filestream;
	filestream.open(CONFIGS"game.yml", fstream::out);

	filestream << out.c_str();

	filestream.close();

}

/********************************** GETTER AND SETTER **********************************************************/

Level* Game::getCurrentLevel() {
	return currentLevel;
}

void Game::setCurrentLevel(Level *curLev) {
	currentLevel = curLev;
}

TTF_Font* Game::getFont(int which) {
	return font[which];
}

