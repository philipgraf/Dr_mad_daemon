#include "Level.h"
#include "Game.h"
#include "Slot.h"
#include "BadGuy.h"
#include "Notification.h"
#include "PDA.h"
#include "Item.h"
#include "Environment.h"

#include <sstream>
#include <fstream>
#include <yaml-cpp/yaml.h>

#include "Notification.h"

using namespace std;

vector<string> Level::levels;
map<string, string> Level::levelnames;

/**
 * load the levelnames from the levelconfigs
 */
void Level::loadLevels() {
	levels.push_back("l000");
	levels.push_back("l001");
	levels.push_back("l002");
	levels.push_back("l003");

	for (unsigned i = 0; i < levels.size(); i++) {
		YAML::Node levelconfig = YAML::LoadFile(LEVELS + levels[i] + ".yml");
		levelnames[levels[i]] = levelconfig["name"].Scalar();
	}

}

/**
 * Initialize all necessary information from the levelconfig
 * create all items, badguys and environments, also create the player and the main camera.
 * @param levelnum the number of the level this is needed to load the correct mapfile.
 */
Level::Level(unsigned levelnum) {

	if (levelnum >= levels.size()) {
		//TODO throw exception
		this->levelnum = 0;
	} else {
		this->levelnum = levelnum;
	}

	Game::curGame->setCurrentLevel(this);

	switches = 0xFF;

	tilelist = NULL;
	bgImage = NULL;
	running = true;
	levelFinished = false;
	string background;
	float gravity;
	YAML::Node levelconfig = YAML::LoadFile(LEVELS + levels[this->levelnum] + ".yml");

	name = levelconfig["name"].Scalar();
	width = levelconfig["width"].as<int>();
	height = levelconfig["height"].as<int>();
	background = levelconfig["bgImage"].Scalar();
	gravity = levelconfig["gravity"].as<float>();
	time = levelconfig["time"].as<int>();
	bgMusic = Mix_LoadMUS((MUSIC + levelconfig["bgMusic"].Scalar()).c_str());

	gravity2d = new b2Vec2(0.0f, gravity * 9.81);
	world = new b2World(*gravity2d);

	Tile::loadTileset();

#if DEBUG >= 3
	b2Debug.SetFlags(b2Draw::e_shapeBit|b2Draw::e_jointBit); // | b2Draw::e_aabbBit);
	world->SetDebugDraw(&b2Debug);
#endif

	SDL_Surface *tmp = SDL_LoadBMP((IMG+background).c_str());

	if (!tmp) {
		cout << "unable to load BMP file" << endl;
	} else {
		this->bgImage = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		if (this->bgImage != 0) {
			SDL_SetColorKey(this->bgImage, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(this->bgImage->format, 255, 0, 255));
		}
	}

	loadMapFile(levels[this->levelnum]);

	player = new Player(levelconfig["player"]["x"].as<int>(), levelconfig["player"]["y"].as<int>(), Slot::slots[Game::curGame->settings.activeSlot]->getPdaLevel());

	YAML::Node badguys = levelconfig["badguys"];

	for (YAML::iterator it = badguys.begin(); it != badguys.end(); ++it) {
		new BadGuy(it->first.Scalar(), it->second["x"].as<int>(), it->second["y"].as<int>());
	}

	YAML::Node items = levelconfig["items"];
	for (YAML::iterator it = items.begin(); it != items.end(); ++it) {
		new Item(it->first.Scalar(), it->second["x"].as<int>(), it->second["y"].as<int>());
	}
	YAML::Node environments = levelconfig["environments"];
	for (YAML::iterator it = environments.begin(); it != environments.end(); ++it) {
		new Environment(it->first.Scalar(), it->second["x"].as<int>(), it->second["y"].as<int>());
	}

	mainCam = new Camera(player);
	timer = SDL_GetTicks();

}

/**
 * free all allocated memory
 */
Level::~Level() {
	for (int i = 0; i < 3; i++) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				delete tilelist[i][x][y];
			}
			delete[] tilelist[i][x];
		}
		delete[] tilelist[i];
	}
	delete[] tilelist;
	for (unsigned i = 0; i < Entity::entityList.size(); i++) {
		delete Entity::entityList[i];
	}

	for (unsigned i = 0; i < Item::itemlist.size(); i++) {
		delete Item::itemlist[i];
	}
	Item::itemlist.clear();

	Entity::entityList.clear();
	SDL_FreeSurface(Tile::tileset);
	Mix_HaltMusic();
	Mix_FreeMusic(bgMusic);
	Game::curGame->setCurrentLevel(NULL);

}

/**
 * load the map file and build the tilelist array.
 */
void Level::loadMapFile(string filename) {
	tilelist = new Tile***[3];
	for (int i = 0; i < 3; i++) {
		tilelist[i] = new Tile**[width];
		for (int j = 0; j < width; j++) {
			tilelist[i][j] = new Tile*[height];
		}
	}
	fstream filestream;

	filestream.open((LEVELS + filename + ".map").c_str(), fstream::in);
	for (int i = 0; i < 3; i++) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				u_int64_t id;
				filestream >> id;

				tilelist[i][x][y] = new Tile(id);

				if (i == 1 && ((id & 0xFFFF) != 0)) {
					b2BodyDef groundBodyDef;
					groundBodyDef.fixedRotation = true;
					groundBodyDef.position.Set(x + 0.5, y + 0.5);
					b2Body* groundBody = world->CreateBody(&groundBodyDef);
					b2PolygonShape groundBox;
					groundBox.SetAsBox(0.5, 0.5);
					groundBody->CreateFixture(&groundBox, 0.0f);
					tilelist[i][x][y]->setBody(groundBody);
				}

				if ((id >> 16) & 0xFF) {
					switches &= ~((id >> 16) & 0xFF);
				}
			}
		}
		char ch;
		filestream >> ch;
		if (ch != ';')
			cout << "Error loading mapfile:"LEVELS + filename + ".map" << endl; // TODO Throw Errorobjekt
	}
	filestream.close();
}

/** Render function from level.
 *	tell the main camera to render the level and flip the surface
 */
void Level::render() {
	mainCam->drawImage();
#if DEBUG >= 3
	world->DrawDebugData();
#endif
	SDL_Flip(SDL_GetVideoSurface());
}

/** Logic function of the level.
 * update the time, call all tile, entity, items, notification and camera logic.
 * also check if the player is alive and if the level is finished.
 */
void Level::logic() {
	static bool entityCreated = false;
	updateTime();
	if (time <= 0) {
		player->setAlive(false);
	}

	if (!player->isAlive()) {
		Menu *gameOverMenu = new Menu(GAMEOVER);
		gameOverMenu->show();
	}
	if (levelnum == 2 && tilelist[1][43][16]->getCurrentframe() == 0 && tilelist[1][43][16]->getId() != 0) {
		if (entityCreated == false) {
			new BadGuy("vacuuBoy", 41, 16);
			entityCreated = true;
		}
	} else {
		entityCreated = false;
	}

	if (levelFinished) {
//		Levelresult levelresult;
//		levelresult.time = this->time;
//		levelresult.items = player->items;
//		levelresult.slot = game->getCurrentSlot;
//		...
//
//		levelresult.save();
//		levelresult.show();

		Slot::slots[Game::curGame->settings.activeSlot]->setPlayerItems(player->getItems());
		Slot::slots[Game::curGame->settings.activeSlot]->setPdaLevel(player->getpda().getLevel());
		Slot::slots[Game::curGame->settings.activeSlot]->checkAndSetFinishedLevels(levelnum);
		Slot::saveSlots();

		running = false;
	}

	for (int layer = 0; layer < 3; layer++) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				tilelist[layer][x][y]->logic();
			}
		}
	}
	//TODO move to a better place and check
	float timeStep = 1.0f / FPS;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world->Step(timeStep, velocityIterations, positionIterations);

	for (unsigned i = 0; i < Entity::entityList.size(); i++) {
		if (Entity::entityList[i]->isAlive()) {
			Entity::entityList[i]->logic();
		}
	}
	mainCam->logic();
	for (vector<Notification*>::iterator it = Notification::notificationList.begin(); it != Notification::notificationList.end(); ++it) {
		(*it)->timeout();
		if (it == Notification::notificationList.end()) {
			break;
		}
	}

	for (unsigned i = 0; i < Item::itemlist.size(); i++) {
		Item::itemlist[i]->logic();
	}
}

/** Play function of the level.
 * the main loop which runs until running is false.
 * @see running
 */
void Level::play() {
	SDL_Event event;
#ifndef DEBUG
	Uint32 start;
#endif
	Mix_PlayMusic(bgMusic, -1);

#if DEBUG >= 1
	int fps=0;
	int fpstime=0;
#endif
	while (running) {
#if DEBUG >=1
		fps++;
#else
		start = SDL_GetTicks();
#endif
		while (SDL_PollEvent(&event)) {
			onEvent(&event);
		}
		logic();
		render();

#ifndef DEBUG
		if (SDL_GetTicks() - start < 1000 / FPS) {

			SDL_Delay(1000 / FPS - (SDL_GetTicks() - start));
		}
#endif
#if DEBUG >= 1

		if(SDL_GetTicks() - fpstime > 1000) {
			stringstream out;
			out << "FPS: " << fps;
			SDL_WM_SetCaption(out.str().c_str(),NULL);
			cout << out.str() << endl;
			fps=0;
			fpstime = SDL_GetTicks();
		}
#endif

	}
}

/**
 * if 1000 milliseconds are gone the time variable will be decreased by one.
 */
void Level::updateTime() {

	if (SDL_GetTicks() - timer > 1000) {
		time--;
		timer = SDL_GetTicks();
	}
}

/**
 * handle the key down events
 * @param sym the symbol of the key
 * @param mod modifications like left-Shift or alt button
 * @param unicode the unicode value of the pressed key
 */
void Level::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {

	if (sym == SDLK_ESCAPE) {
		Menu *pauseMenu = new Menu(PAUSEMENU);
		if (pauseMenu->show() == -1) {
			//TODO only temporary! need to free all allocated memory and so on
			exit(0);
		}
		delete pauseMenu;
		player->delDirection(0xFF);
	} else if (sym == Game::curGame->settings.keyboard.left) {
		player->setDirection(LEFT);
	} else if (sym == Game::curGame->settings.keyboard.jump) {
		player->setJumping(true);
	} else if (sym == Game::curGame->settings.keyboard.down) {
		player->setDirection(DOWN);
	} else if (sym == Game::curGame->settings.keyboard.right) {
		player->setDirection(RIGHT);
	} else if (sym == Game::curGame->settings.keyboard.run) {
		player->setRunning(true);
	} else if (sym == Game::curGame->settings.keyboard.use) {
		player->use();
	} else if (sym == Game::curGame->settings.keyboard.pda) {
		player->pda.show();
		player->delDirection(0xFF);
	} else if (sym == Game::curGame->settings.keyboard.grab) {
		player->grab();
	} else if (sym == Game::curGame->settings.keyboard.up) {
		player->setDirection(UP);
	}
}

/**
 * handle the key up events
 * @param sym the symbol of the key
 * @param mod modifications like left-Shift or alt button
 * @param unicode the unicode value of the pressed key
 */
void Level::onKeyUP(SDLKey sym, SDLMod mod, Uint16 unicode) {
	if (sym == Game::curGame->settings.keyboard.left) {
		player->delDirection(LEFT);
	} else if (sym == Game::curGame->settings.keyboard.jump) {
		player->setJumping(false);
	} else if (sym == Game::curGame->settings.keyboard.down) {
		player->delDirection(DOWN);
	} else if (sym == Game::curGame->settings.keyboard.right) {
		player->delDirection(RIGHT);
	} else if (sym == Game::curGame->settings.keyboard.run) {
		player->setRunning(false);
	} else if (sym == Game::curGame->settings.keyboard.up) {
		player->delDirection(UP);
	}
}

/**
 * handle the wii button events
 */
void Level::onWiiButtonEvent(int button) {
	if (button == WII_BTN_HOME) {
		Menu *pauseMenu = new Menu(PAUSEMENU);
		if (pauseMenu->show() == -1) {
			//TODO only temporary! need to free all allocated memory and so on
			exit(0);
		}
		delete pauseMenu;
		player->delDirection(0xFF);
	}

	if (button & Game::curGame->settings.wiimote.left) {
		player->setDirection(LEFT);
	} else {
		player->delDirection(LEFT);
	}

	if (button & Game::curGame->settings.wiimote.jump) {
		player->setJumping(true);
	} else {
		player->setJumping(false);
	}

	if (button & Game::curGame->settings.wiimote.down) {
		player->setDirection(DOWN);
	} else {
		player->delDirection(DOWN);
	}

	if (button & Game::curGame->settings.wiimote.right) {
		player->setDirection(RIGHT);
	} else {
		player->delDirection(RIGHT);
	}
	if (button & Game::curGame->settings.wiimote.run) {
		player->setRunning(true);
	} else {
		player->setRunning(false);
	}

	if (button & Game::curGame->settings.wiimote.use) {
		player->use();
	}

	if (button & Game::curGame->settings.wiimote.pda) {
		player->pda.show();
		player->delDirection(0xFF);
	}

	if (button & Game::curGame->settings.wiimote.grab) {
		player->grab();
		player->delDirection(0xFF);
	}
}

/** Call the right level-Switch logic
 * call the level logic depend on the levelnumber.
 * @see level0logic()
 * @see level1logic()
 * @see level2logic()
 * @see level3logic()
 * @see levelnum()
 */
void Level::switchActions() {
	switch (levelnum) {
	case 0:
		level0Logic();
		break;
	case 1:
		level1Logic();
		break;
	case 2:
		level2Logic();
		break;
	case 3:
		level3Logic();
		break;
	}
}

/******************************** LEVEL DEPENDENT SWITCH LOGICS ************************/

/** Switch logic of level0.
 * switch1 (paystation) if slot is in the inventory, turn lights to green and and switch the flags of the door to finish
 */
void Level::level0Logic() {
	if (switches & TF_SWITCH1) {
		if (player->getItems()["slot"] < 1) {
			switches &= ~TF_SWITCH1;	//unset switch1 if slot is not in the inventory
			new Notification(lang["item missing"], 5);
		} else {
			switches = 0xFF;
			// switch hand to green
			tilelist[0][27][16]->setCurrentframe(1);

			// switch checkpoint lights to green
			tilelist[0][34][16]->setCurrentframe(2);
			tilelist[0][38][16]->setCurrentframe(2);

			// change tileflag of the doors to finish
			tilelist[0][35][16]->setFlags(TF_FINISH);
			tilelist[0][36][16]->setFlags(TF_FINISH);
			tilelist[0][37][16]->setFlags(TF_FINISH);

		}
	} else {
		// switch hand to red
		tilelist[0][27][16]->setCurrentframe(0);

		// turn off checkpoint lights
		tilelist[0][34][16]->setCurrentframe(0);
		tilelist[0][38][16]->setCurrentframe(0);
	}

	if ((switches & TF_SWITCH2) && !(switches & TF_SWITCH1)) {
		new Notification(lang["pay first"], 5);

		// switch checkpoint lights to red
		tilelist[0][34][16]->setCurrentframe(1);
		tilelist[0][38][16]->setCurrentframe(1);
	}

}

/** Switch logic of level1.
 * switch1 (hand) open the door and change the hand to green.
 */
void Level::level1Logic() {
	if (switches & TF_SWITCH1) {
		//switch hand to green
		tilelist[0][97][16]->setCurrentframe(1);

		// open door
		tilelist[0][96][15]->setCurrentframe(1);
		tilelist[0][96][16]->setCurrentframe(1);
		tilelist[0][96][17]->setCurrentframe(1);

		//add finish flag
		tilelist[0][96][15]->setFlags(TF_FINISH);
		tilelist[0][96][16]->setFlags(TF_FINISH);
		tilelist[0][96][17]->setFlags(TF_FINISH);
	} else {
		//switch hand to green
		tilelist[0][97][16]->setCurrentframe(0);

		// open door
		tilelist[0][96][15]->setCurrentframe(0);
		tilelist[0][96][16]->setCurrentframe(0);
		tilelist[0][96][17]->setCurrentframe(0);

		//add finish flag
		tilelist[0][96][15]->setFlags(0);
		tilelist[0][96][16]->setFlags(0);
		tilelist[0][96][17]->setFlags(0);
	}

}

/**
 * Switch logic of level2.
 * switch1 (main switch) ibuild run away and door is open
 */
void Level::level2Logic() {
	if (switches & TF_SWITCH1) {
		if (tilelist[1][42][15]->getId() != 0) {
			//remove iBuild
			tilelist[1][42][15]->setId(0);
			tilelist[1][42][16]->setId(0);
			tilelist[1][42][17]->setId(0);
			tilelist[1][43][15]->setId(0);
			tilelist[1][43][16]->setId(0);
			tilelist[1][43][17]->setId(0);
			tilelist[1][44][15]->setId(0);
			tilelist[1][44][16]->setId(0);
			tilelist[1][44][17]->setId(0);

			world->DestroyBody(tilelist[1][42][15]->getBody());
			world->DestroyBody(tilelist[1][42][16]->getBody());
			world->DestroyBody(tilelist[1][42][17]->getBody());
			world->DestroyBody(tilelist[1][43][15]->getBody());
			world->DestroyBody(tilelist[1][43][16]->getBody());
			world->DestroyBody(tilelist[1][43][17]->getBody());
			world->DestroyBody(tilelist[1][44][15]->getBody());
			world->DestroyBody(tilelist[1][44][16]->getBody());
			world->DestroyBody(tilelist[1][44][17]->getBody());

			tilelist[0][44][15]->setCurrentframe(1);
			tilelist[0][44][16]->setCurrentframe(1);
			tilelist[0][44][17]->setCurrentframe(1);
			tilelist[0][45][15]->setCurrentframe(1);
			tilelist[0][45][16]->setCurrentframe(1);
			tilelist[0][45][17]->setCurrentframe(1);
			tilelist[0][46][15]->setCurrentframe(1);
			tilelist[0][46][16]->setCurrentframe(1);
			tilelist[0][46][17]->setCurrentframe(1);

			tilelist[0][44][15]->setFlags(TF_FINISH);
			tilelist[0][44][16]->setFlags(TF_FINISH);
			tilelist[0][44][17]->setFlags(TF_FINISH);
			tilelist[0][45][15]->setFlags(TF_FINISH);
			tilelist[0][45][16]->setFlags(TF_FINISH);
			tilelist[0][45][17]->setFlags(TF_FINISH);
			tilelist[0][46][15]->setFlags(TF_FINISH);
			tilelist[0][46][16]->setFlags(TF_FINISH);
			tilelist[0][46][17]->setFlags(TF_FINISH);
		}
	}else {
		tilelist[0][44][15]->setCurrentframe(0);
		tilelist[0][44][16]->setCurrentframe(0);
		tilelist[0][44][17]->setCurrentframe(0);
		tilelist[0][45][15]->setCurrentframe(0);
		tilelist[0][45][16]->setCurrentframe(0);
		tilelist[0][45][17]->setCurrentframe(0);
		tilelist[0][46][15]->setCurrentframe(0);
		tilelist[0][46][16]->setCurrentframe(0);
		tilelist[0][46][17]->setCurrentframe(0);

		tilelist[0][44][15]->setFlags(0);
		tilelist[0][44][16]->setFlags(0);
		tilelist[0][44][17]->setFlags(0);
		tilelist[0][45][15]->setFlags(0);
		tilelist[0][45][16]->setFlags(0);
		tilelist[0][45][17]->setFlags(0);
		tilelist[0][46][15]->setFlags(0);
		tilelist[0][46][16]->setFlags(0);
		tilelist[0][46][17]->setFlags(0);
	}
}

void Level::level3Logic() {

}

/******************************* GETTER / SETTER ***************************************/

/** Get the gravity of the level.
 * @return the y value of the gravity vector
 */
int Level::getGravity() const {
	return gravity2d->y;
}

/**
 * set the gravity to given value.
 * @param gravity the new gravity value
 */
void Level::setGravity(int gravity) {
	this->gravity2d->y = gravity;
}

const string& Level::getName() const {
	return name;
}

int Level::getTime() const {
	return time;
}

void Level::setTime(int time) {
	this->time = time;
}

int Level::getTileID(int x, int y, int layer) {
	return tilelist[layer][x][y]->getId();
}

Player* Level::getPlayer() {
	return player;
}

int Level::getHeight() const {
	return height;
}

int Level::getWidth() const {
	return width;
}

Tile**** Level::getTilelist() const {
	return tilelist;
}

SDL_Surface* Level::getBackground() const {
	return bgImage;
}

bool Level::isFinished() const {
	return levelFinished;
}

void Level::setFinished(bool finished) {
	this->levelFinished = finished;
}

void Level::setRunning(bool running) {
	this->running = running;
}

b2World* Level::getWorld() const {
	return world;
}

Uint8 Level::getSwitches() const {
	return switches;
}

void Level::toggleSwitch(Uint8 flags) {
	switches ^= flags;
}
