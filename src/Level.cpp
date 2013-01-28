#include "Level.h"

Level::Level(string lname) {

	Game::curGame->setCurrentLevel(this);

	tilelist = NULL;
	background = NULL;

	string background;

	fstream filestream;
	filestream.open((LEVELS + lname + ".conf").c_str(), fstream::in);
	filestream >> name >> width >> height >> background >> gravity >> time;
	filestream.close();

	gravity2d = new b2Vec2(0.0f, gravity*10);
	world = new b2World(*gravity2d);

	background = IMG + background;

	this->background = Tools::loadImage(background);

	tilelist = new Tile***[3];
	for (int i = 0; i < 3; i++) {
		tilelist[i] = new Tile**[width];
		for (int j = 0; j < width; j++) {
			tilelist[i][j] = new Tile*[height];
		}
	}

	filestream.open((LEVELS + lname + ".map").c_str(), fstream::in);
	for (int i = 0; i < 3; i++) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				u_int64_t id;
				filestream >> id;

				if (id & TF_START) {
					//TODO: get width and height dynamically
					player = new Entity(IMG "player.png", 1, 2, x, y);
				}

				tilelist[i][x][y] = new Tile(id);

				if (i == 1 && ((id & 0xFFFF) != 0)) {
					b2BodyDef groundBodyDef;
					groundBodyDef.fixedRotation=true;
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
			Tools::error("Error loading mapfile:"LEVELS + lname + ".map"); // TODO Throw Errorobjekt
	}
	filestream.close();
//
//	b2BodyDef groundBodyDef;
//	groundBodyDef.fixedRotation = true;
//	groundBodyDef.position.Set(0, 16);
//	b2Body* groundBody = world->CreateBody(&groundBodyDef);
//
//	b2ChainShape ground;
//
//	b2Vec2 v[6];
//	v[0].Set(0,0);
//	v[1].Set(13,0);
//	v[2].Set(13,-2);
//	v[3].Set(15,-2);
//	v[4].Set(15,0);
//	v[5].Set(24,0);
//
//	ground.CreateChain(v,6);
//
//
//
//	groundBody->CreateFixture(&ground,0.0f);

	mainCam = new Camera(player);
}

Level::~Level() {
	for (int i = 0; i < 3; i++) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				delete tilelist[i][y][x];
			}
			delete[] tilelist[i][y];
		}
		delete[] tilelist[i];
	}
	delete[] tilelist;
}

int Level::getGravity() const {
	return gravity;
}

void Level::setGravity(int gravity) {
	this->gravity = gravity;
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

void Level::render() {

	mainCam->drawImage();

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
	float32 timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world->Step(timeStep, velocityIterations, positionIterations);

	player->move();
	mainCam->logic();

}
