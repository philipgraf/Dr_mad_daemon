#include "Level.h"
#include "Game.h"
#include "Slot.h"
#include "BadGuy.h"
#include "sstream"
#include <fstream>
#include <yaml-cpp/yaml.h>

vector<string> Level::levels;
map<string, string> Level::levelnames;

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

Level::Level(unsigned levelnum) {

	if (levelnum >= levels.size()) {
		//TODO throw exception
		this->levelnum = 0;
	} else {
		this->levelnum = levelnum;
	}

	//TODO set current level???
	Game::curGame->setCurrentLevel(this);

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
	b2Debug.SetFlags(b2Draw::e_shapeBit); // | b2Draw::e_aabbBit);
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

	mainCam = new Camera(player);
}

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
	Entity::entityList.clear();
	SDL_FreeSurface(Tile::tileset);
}

void Level::loadMapFile(string filename) {

	//TODO only testing
	BadGuy *test = new BadGuy("vacuuBoy", 5, 5);

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

				if (id & TF_START) {
					//TODO: get width and height dynamically
					player = new Player(x, y);
				}

				tilelist[i][x][y] = new Tile(id);

				if (i == 1 && ((id & 0xFFFF) != 0)) {
					b2BodyDef groundBodyDef;
					groundBodyDef.fixedRotation = true;
					groundBodyDef.position.Set(x + 0.5, y + 0.5);
					b2Body* groundBody = world->CreateBody(&groundBodyDef);
					b2PolygonShape groundBox;
					groundBox.SetAsBox(0.5, 0.5);

					groundBody->CreateFixture(&groundBox, 0.0f);
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

void Level::render() {

	mainCam->drawImage();
#if DEBUG >= 3
	world->DrawDebugData();
#endif
	SDL_Flip(SDL_GetVideoSurface());
}

void Level::logic() {

	if (levelFinished) {
//		Levelresult levelresult;
//		levelresult.time = this->time;
//		levelresult.items = player->items;
//		levelresult.slot = game->getCurrentSlot;
//		...
//
//		levelresult.save();
//		levelresult.show();

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
	float32 timeStep = 1.0f / FPS;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world->Step(timeStep, velocityIterations, positionIterations);

	for (unsigned i = 0; i < Entity::entityList.size(); i++) {
		Entity::entityList[i]->logic();
	}
	mainCam->logic();

}

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
	Mix_HaltMusic();
	Mix_FreeMusic(bgMusic);
	Game::curGame->destroyCurrentLevel();
}

void Level::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {

	if (sym == SDLK_ESCAPE) {

		//TODO check out
		Menu *pauseMenu;
		pauseMenu = new Menu(PAUSEMENU);
		pauseMenu->show();
		delete pauseMenu;
	} else if (sym == Game::curGame->settings.controller.left) {
		player->setDirection(LEFT);
	} else if (sym == Game::curGame->settings.controller.jump) {
		player->setDirection(UP);
	} else if (sym == Game::curGame->settings.controller.down) {
		player->setDirection(DOWN);
	} else if (sym == Game::curGame->settings.controller.right) {
		player->setDirection(RIGHT);
	} else if (sym == Game::curGame->settings.controller.run) {
		player->setRunning(true);
	} else if (sym == Game::curGame->settings.controller.use) {
		player->use();
	}

}

void Level::onKeyUP(SDLKey sym, SDLMod mod, Uint16 unicode) {
	if (sym == Game::curGame->settings.controller.left) {
		player->delDirection(LEFT);
	} else if (sym == Game::curGame->settings.controller.jump) {
		player->delDirection(UP);
	} else if (sym == Game::curGame->settings.controller.down) {
		player->delDirection(DOWN);
	} else if (sym == Game::curGame->settings.controller.right) {
		player->delDirection(RIGHT);
	} else if (sym == Game::curGame->settings.controller.run) {
		player->setRunning(false);
	}
}

/******************************* GETTER / SETTER ***************************************/

int Level::getGravity() const {
	return gravity2d->y;
}

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
