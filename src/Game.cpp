
#include "Game.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

#include "Slot.h"

using namespace std;

Game* Game::curGame;
map<string, Mix_Chunk*> Game::sounds;
/**
 * Constructor of the Game
 */
Game::Game() {
	curGame = this;
	display = NULL;
	currentLevel = NULL;

}
/**
 * Destructor of the Game
*/
Game::~Game() {
	SDL_FreeSurface(display);
	SDL_Quit();

	for (map<string, Mix_Chunk*>::iterator it = sounds.begin(); it != sounds.end(); ++it) {
		Mix_FreeChunk((*it).second);
	}

	sounds.clear();

	Language::supLanguages.clear();
	Level::levels.clear();
	Level::levelnames.clear();

	Mix_CloseAudio();
	Mix_Quit();
}
/**
 * Starts the Game.
 * This method calls the Game::init() to initialize the game and generates the mainmenu
 * If mainmenu is left it will also show a goodbye message
 */
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

/**
 * This will initialize many things
 * Here is where all the loading methods are called, fonts get defined. Also sound and video (screen) get configured
 */
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
	if ((display = SDL_SetVideoMode(WIDTH, HEIGHT, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_SWSURFACE)) == NULL) {
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
	if ((font[FONT_PDA_CLOCK_SMALL] = TTF_OpenFont(FONTS "digital.ttf", 12)) == NULL) {
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

	Mix_VolumeMusic(settings.volume / 2);
	Mix_Volume(-1, settings.volume);

	/**
	 * load all the sound files
	 */
	loadSounds();

	Level::loadLevels();

	Slot::loadSlots();

	Language::getSupportedLanguages();

	PDA::loadRequirements();
}

/**
 * Load the gamesettings from YAML-file
 */
void Game::loadSettings() {
	string home = "";
	if (getenv("HOME") != NULL) {
		home = getenv("HOME");
	} else {
		home = ".";
	}
	YAML::Node settings;
	try {
		settings = YAML::LoadFile(home + "/.DIS/game.yml");
	} catch (YAML::Exception &e) {
		cout << e.msg << endl;
	}

	if (settings["language"].Scalar() == "") {
		this->settings.language = "en";
	} else {
		this->settings.language = settings["language"].Scalar();
	}
	if (settings["audio rate"].Scalar() == "") {
		this->settings.audioRate = 22050;
	} else {
		this->settings.audioRate = settings["audio rate"].as<int>();
	}
	if (settings["volume"].Scalar() == "") {
		this->settings.volume = 64;
	} else {
		this->settings.volume = settings["volume"].as<int>();
	}
	if (settings["active slot"].Scalar() == "") {
		this->settings.activeSlot = -1;
	} else {
		this->settings.activeSlot = settings["active slot"].as<int>();
	}

	if (settings["control"].Scalar() == "") {
		this->settings.keyboard.left = SDLK_a;
		this->settings.keyboard.right = SDLK_d;
		this->settings.keyboard.up = SDLK_w;
		this->settings.keyboard.down = SDLK_s;
		this->settings.keyboard.run = SDLK_LSHIFT;
		this->settings.keyboard.jump = SDLK_SPACE;
		this->settings.keyboard.use = SDLK_e;
		this->settings.keyboard.pda = SDLK_p;
		this->settings.keyboard.grab = SDLK_f;
		this->settings.wiimote.left = WII_BTN_UP;
		this->settings.wiimote.right = WII_BTN_DOWN;
		this->settings.wiimote.up = WII_BTN_RIGHT;
		this->settings.wiimote.down = WII_BTN_LEFT;
		this->settings.wiimote.run = WII_BTN_1;
		this->settings.wiimote.jump = WII_BTN_2;
		this->settings.wiimote.use = WII_BTN_A;
		this->settings.wiimote.pda = WII_BTN_PLUS;
		this->settings.wiimote.grab = WII_BTN_B;
	} else {
		if (settings["control"]["keyboard"].Scalar() == "") {
			this->settings.keyboard.left = SDLK_a;
			this->settings.keyboard.right = SDLK_d;
			this->settings.keyboard.up = SDLK_w;
			this->settings.keyboard.down = SDLK_s;
			this->settings.keyboard.run = SDLK_LSHIFT;
			this->settings.keyboard.jump = SDLK_SPACE;
			this->settings.keyboard.use = SDLK_e;
			this->settings.keyboard.pda = SDLK_p;
			this->settings.keyboard.grab = SDLK_f;
		} else {
			if (settings["control"]["keyboard"]["left"].Scalar() == "") {
				this->settings.keyboard.left = SDLK_a;
			} else {
				this->settings.keyboard.left = settings["control"]["keyboard"]["left"].as<int>();
			}

			if (settings["control"]["keyboard"]["right"].Scalar() == "") {
				this->settings.keyboard.right = SDLK_d;
			} else {
				this->settings.keyboard.right = settings["control"]["keyboard"]["right"].as<int>();
			}

			if (settings["control"]["keyboard"]["up"].Scalar() == "") {
				this->settings.keyboard.up = SDLK_w;
			} else {
				this->settings.keyboard.up = settings["control"]["keyboard"]["up"].as<int>();
			}

			if (settings["control"]["keyboard"]["down"].Scalar() == "") {
				this->settings.keyboard.down = SDLK_s;
			} else {
				this->settings.keyboard.down = settings["control"]["keyboard"]["down"].as<int>();
			}

			if (settings["control"]["keyboard"]["run"].Scalar() == "") {
				this->settings.keyboard.run = SDLK_LSHIFT;
			} else {
				this->settings.keyboard.run = settings["control"]["keyboard"]["run"].as<int>();
			}

			if (settings["control"]["keyboard"]["jump"].Scalar() == "") {
				this->settings.keyboard.jump = SDLK_SPACE;
			} else {
				this->settings.keyboard.jump = settings["control"]["keyboard"]["jump"].as<int>();
			}

			if (settings["control"]["keyboard"]["use"].Scalar() == "") {
				this->settings.keyboard.use = SDLK_e;
			} else {
				this->settings.keyboard.use = settings["control"]["keyboard"]["use"].as<int>();
			}

			if (settings["control"]["keyboard"]["pda"].Scalar() == "") {
				this->settings.keyboard.pda = SDLK_p;
			} else {
				this->settings.keyboard.pda = settings["control"]["keyboard"]["pda"].as<int>();
			}

			if (settings["control"]["keyboard"]["grab"].Scalar() == "") {
				this->settings.keyboard.grab = SDLK_f;
			} else {
				this->settings.keyboard.grab = settings["control"]["keyboard"]["grab"].as<int>();
			}
		}

		if (settings["control"]["wiimote"].Scalar() == "") {
			this->settings.wiimote.left = WII_BTN_UP;
			this->settings.wiimote.right = WII_BTN_DOWN;
			this->settings.wiimote.up = WII_BTN_RIGHT;
			this->settings.wiimote.down = WII_BTN_LEFT;
			this->settings.wiimote.run = WII_BTN_1;
			this->settings.wiimote.jump = WII_BTN_2;
			this->settings.wiimote.use = WII_BTN_A;
			this->settings.wiimote.pda = WII_BTN_PLUS;
			this->settings.wiimote.grab = WII_BTN_B;
		} else {
			if (settings["control"]["wiimote"]["left"].Scalar() == "") {
				this->settings.keyboard.left = WII_BTN_UP;
			} else {
				this->settings.keyboard.left = settings["control"]["wiimote"]["left"].as<int>();
			}

			if (settings["control"]["wiimote"]["right"].Scalar() == "") {
				this->settings.keyboard.right = WII_BTN_DOWN;
			} else {
				this->settings.keyboard.right = settings["control"]["wiimote"]["right"].as<int>();
			}

			if (settings["control"]["wiimote"]["up"].Scalar() == "") {
				this->settings.keyboard.up = WII_BTN_RIGHT;
			} else {
				this->settings.keyboard.up = settings["control"]["wiimote"]["up"].as<int>();
			}

			if (settings["control"]["wiimote"]["down"].Scalar() == "") {
				this->settings.keyboard.down = WII_BTN_LEFT;
			} else {
				this->settings.keyboard.down = settings["control"]["wiimote"]["down"].as<int>();
			}

			if (settings["control"]["wiimote"]["run"].Scalar() == "") {
				this->settings.keyboard.run = WII_BTN_1;
			} else {
				this->settings.keyboard.run = settings["control"]["wiimote"]["run"].as<int>();
			}

			if (settings["control"]["wiimote"]["jump"].Scalar() == "") {
				this->settings.keyboard.jump = WII_BTN_2;
			} else {
				this->settings.keyboard.jump = settings["control"]["wiimote"]["jump"].as<int>();
			}

			if (settings["control"]["wiimote"]["use"].Scalar() == "") {
				this->settings.keyboard.use = WII_BTN_A;
			} else {
				this->settings.keyboard.use = settings["control"]["wiimote"]["use"].as<int>();
			}
			if (settings["control"]["wiimote"]["pda"].Scalar() == "") {
				this->settings.keyboard.pda = WII_BTN_PLUS;
			} else {
				this->settings.keyboard.pda = settings["control"]["wiimote"]["pda"].as<int>();
			}

			if (settings["control"]["wiimote"]["grab"].Scalar() == "") {
				this->settings.keyboard.grab = WII_BTN_B;
			} else {
				this->settings.keyboard.grab = settings["control"]["wiimote"]["grab"].as<int>();
			}
		}

	}

}

/**
 * Load all soundfiles
 */
void Game::loadSounds() {
	sounds["menu select"] = Mix_LoadWAV(SOUNDS"menu_select.ogg");
	sounds["menu confirm"] = Mix_LoadWAV(SOUNDS"menu_confirm.ogg");
	sounds["player jump"] = Mix_LoadWAV(SOUNDS"player_jump.ogg");
	sounds["player jump impact"] = Mix_LoadWAV(SOUNDS"player_jump_impact.ogg");
}
/**
 * Save all settings back to YAML-file
 */
void Game::saveSettings() {

	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "language";
	out << YAML::Value << settings.language;
	out << YAML::Key << "audio rate";
	out << YAML::Value << settings.audioRate;
	out << YAML::Key << "active slot";
	out << YAML::Value << settings.activeSlot;
	out << YAML::Key << "volume";
	out << YAML::Value << (unsigned) settings.volume;
	out << YAML::Key << "control";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "keyboard";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "left";
	out << YAML::Value << settings.keyboard.left;
	out << YAML::Key << "right";
	out << YAML::Value << settings.keyboard.right;
	out << YAML::Key << "up";
	out << YAML::Value << settings.keyboard.up;
	out << YAML::Key << "down";
	out << YAML::Value << settings.keyboard.down;
	out << YAML::Key << "run";
	out << YAML::Value << settings.keyboard.run;
	out << YAML::Key << "jump";
	out << YAML::Value << settings.keyboard.jump;
	out << YAML::Key << "use";
	out << YAML::Value << settings.keyboard.use;
	out << YAML::Key << "pda";
	out << YAML::Value << settings.keyboard.pda;
	out << YAML::Key << "grab";
	out << YAML::Value << settings.keyboard.grab;
	out << YAML::EndMap;
	out << YAML::Key << "wiimote";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "left";
	out << YAML::Value << settings.wiimote.left;
	out << YAML::Key << "right";
	out << YAML::Value << settings.wiimote.right;
	out << YAML::Key << "up";
	out << YAML::Value << settings.wiimote.up;
	out << YAML::Key << "down";
	out << YAML::Value << settings.wiimote.down;
	out << YAML::Key << "run";
	out << YAML::Value << settings.wiimote.run;
	out << YAML::Key << "jump";
	out << YAML::Value << settings.wiimote.jump;
	out << YAML::Key << "use";
	out << YAML::Value << settings.wiimote.use;
	out << YAML::Key << "pda";
	out << YAML::Value << settings.wiimote.pda;
	out << YAML::Key << "grab";
	out << YAML::Value << settings.wiimote.grab;
	out << YAML::EndMap;
	out << YAML::EndMap;

	out << YAML::EndMap;

	string home = "";
	if (getenv("HOME") != NULL) {
		home = getenv("HOME");
	} else {
		home = ".";
	}

	fstream filestream;
	filestream.open((home + "/.DIS/game.yml").c_str(), fstream::out);

	filestream << out.c_str();

	filestream.close();

}

/********************************** GETTER AND SETTER **********************************************************/
/**
 * Returns a pointer to the current Level.
 *
 * @returns current Level
 */
Level* Game::getCurrentLevel() {
	return currentLevel;
}

/**
 * Sets a Pointer to the current Level.
 */
void Game::setCurrentLevel(Level *curLev) {
	currentLevel = curLev;
}

/**
 * Returns a pointer to a Font stored in the font array
 *
 * @param which int value for selecting a font
 *
 * @returns font

 */
TTF_Font* Game::getFont(int which) {
	return font[which];
}

