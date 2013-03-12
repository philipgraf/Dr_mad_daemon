/*
 * Environment.cpp
 *
 *  Created on: 28.02.2013
 *      Author: philip
 */

#include <Box2D/Box2D.h>
#include <SDL/SDL.h>
#include <yaml-cpp/yaml.h>
#include <map>

#include "Environment.h"
#include "define.h"
#include "Game.h"

using namespace std;

Environment::Environment(string type, int x, int y) :
		Entity() {
	alive = false;

	YAML::Node environments = YAML::LoadFile(CONFIGS"environment.yml");

	YAML::Node environment = environments[type];

	width = environment["width"].as<float>();
	height = environment["height"].as<float>();

	items = environment["items"].as<map<string, int> >();
	actionframes = environment["action frames"].as<vector<int> >();
	animationDuration = environment["animation duration"].as<vector<unsigned> >();

	float halfWidth = width / 2;
	float halfHeight = height / 2;
	SDL_Surface *tmp = SDL_LoadBMP((ENV+environment["image"].Scalar()).c_str());

	if (!tmp) {
		//TODO Throw Exception
		cout << "unable to load BMP file: " << environment["image"].Scalar() << endl;
	} else {
		image = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		if (image != 0) {
			SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(image->format, 255, 0, 255));
		}
	}
	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.fixedRotation = true;
	bodydef.position.Set(x + halfWidth, y + halfHeight);
	this->body = Game::curGame->getCurrentLevel()->getWorld()->CreateBody(&bodydef);

	body->SetUserData(this);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(halfWidth, halfHeight);
	b2FixtureDef *fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &dynamicBox;

	if(type == "wooden crate"){
		fixtureDef->density = 30.0;
	}else if (type == "matal create"){
		fixtureDef->density = 150.0;
	}else if (type == "matal create 2"){
		fixtureDef->density = 90.0;
	}else{
		fixtureDef->density = 60.0;
	}

	fixtureDef->friction = 0.5;
	body->CreateFixture(fixtureDef);

	b2PolygonShape sensorRight;
	sensorRight.SetAsBox(0.01, halfHeight - 0.1, b2Vec2(halfWidth, 0), 0);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &sensorRight;
	fixtureDef->isSensor = true;
	this->sensorRight = body->CreateFixture(fixtureDef);

	b2PolygonShape sensorLeft;
	sensorLeft.SetAsBox(0.01, halfHeight - 0.1, b2Vec2(-halfWidth, 0), 0);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &sensorLeft;
	fixtureDef->isSensor = true;
	this->sensorLeft = body->CreateFixture(fixtureDef);

	b2PolygonShape sensorTop;
	sensorTop.SetAsBox(halfWidth - 0.1, 0.01, b2Vec2(0, -halfHeight), 0);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &sensorTop;
	fixtureDef->isSensor = true;
	this->sensorTop = body->CreateFixture(fixtureDef);

	b2PolygonShape sensorBottom;
	sensorBottom.SetAsBox(halfWidth - 0.1, 0.01, b2Vec2(0, halfHeight), 0);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &sensorBottom;
	fixtureDef->isSensor = true;
	this->sensorBottom = body->CreateFixture(fixtureDef);

	grounded = false;

}

Environment::~Environment() {
	// TODO Auto-generated destructor stub
}

