#include "Level.h"

Level::Level(string lname) {

	//TODO set current level???
	Game::curGame->setCurrentLevel(this);

	tilelist = NULL;
	background = NULL;
	running = true;
	string background;
	float gravity;
	fstream filestream;
	filestream.open((LEVELS + lname + ".conf").c_str(), fstream::in);
	filestream >> name >> width >> height >> background >> gravity >> time;
	filestream.close();

	gravity2d = new b2Vec2(0.0f, gravity * 10);
	world = new b2World(*gravity2d);

	Tile::loadTileset();

#ifdef DEBUG
	b2Debug.SetFlags(b2Draw::e_shapeBit); // | b2Draw::e_aabbBit);
	world->SetDebugDraw(&b2Debug);
#endif

	SDL_Surface *tmp = SDL_LoadBMP((IMG+background).c_str());

	if (!tmp) {
		cout << "unable to load BMP file" << endl;
	} else {
		this->background = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		if (this->background != 0) {
			SDL_SetColorKey(this->background, SDL_SRCCOLORKEY | SDL_RLEACCEL,
					SDL_MapRGB(this->background->format, 255, 0, 255));
		}
	}

	loadMapFile(lname);

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
	Entity::entityList.clear();
	SDL_FreeSurface(Tile::tileset);
}

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

				if (id & TF_START) {
					//TODO: get width and height dynamically
					player = new Entity("player.bmp", 1, 2, x, y);
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
#ifdef DEBUG
	world->DrawDebugData();
#endif
	SDL_Flip (SDL_GetVideoSurface());
}

void Level::logic() {

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

	player->move();
	mainCam->logic();

}

void Level::play() {
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
			cout << buffer << endl;
			fps=0;
			fpstime = SDL_GetTicks();
		}
#endif

	}
	Game::curGame->destroyCurrentLevel();
}


void Level::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {

	switch (sym) {
	case SDLK_ESCAPE:
		//TODO check out
		Menu *pauseMenu;
		pauseMenu = new Menu(PAUSEMENU);
		pauseMenu->show();
		delete pauseMenu;
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

void Level::onKeyUP(SDLKey sym, SDLMod mod, Uint16 unicode) {
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

Entity* Level::getPlayer() {
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
	return background;
}

void Level::setRunning(bool running) {
	this->running = running;
}

b2World* Level::getWorld() const {
	return world;
}
