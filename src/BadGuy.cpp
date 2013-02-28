/*
 * BadGuy.cpp
 *
 *  Created on: 12.02.2013
 *      Author: philip
 */

#include <iostream>
#include <Box2D/Box2D.h>
#include <SDL/SDL.h>
#include <yaml-cpp/yaml.h>
#include <map>
#include <cstdlib>
#include <ctime>

#include "Game.h"
#include "BadGuy.h"
#include "define.h"
#include "Item.h"

using namespace std;

BadGuy::BadGuy(string type, int x, int y) :
		Entity() {
	YAML::Node badguys = YAML::LoadFile(CONFIGS"badguy.yml");

	YAML::Node badguy = badguys[type];

	maxVelocity = badguy["max velocity"].as<float>();
	width = badguy["width"].as<float>();
	height = badguy["height"].as<float>();

	items = badguy["items"].as<map<string, int> >();
	actionframes = badguy["action frames"].as<vector<int> >();
	animationDuration = badguy["animation duration"].as<vector<unsigned> >();

	float halfWidth = width / 2;
	float halfHeight = height / 2;
	float radius = 0.1;
	SDL_Surface *tmp = SDL_LoadBMP((IMG+badguy["image"].Scalar()).c_str());

	if (!tmp) {
		//TODO Throw Exception
		cout << "unable to load BMP file: " << badguy["image"].Scalar() << endl;
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
	dynamicBox.SetAsBox(halfWidth, halfHeight - radius);
	b2FixtureDef *fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &dynamicBox;
	fixtureDef->density = 35.0;
	fixtureDef->friction = 0.3;
	body->CreateFixture(fixtureDef);

	b2CircleShape leftWheelShape;
	leftWheelShape.m_radius = radius;
	leftWheelShape.m_p = b2Vec2(-halfWidth + radius, halfHeight - radius);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &leftWheelShape;
	fixtureDef->density = 1.0f;
	fixtureDef->friction = 0.2;

	wheels.push_back(body->CreateFixture(fixtureDef));

	b2CircleShape rightWheelShape;
	rightWheelShape.m_radius = radius;
	rightWheelShape.m_p = b2Vec2(+halfWidth - radius, halfHeight - radius);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &rightWheelShape;
	fixtureDef->density = 1.0f;
	fixtureDef->friction = 0.2;

	wheels.push_back(body->CreateFixture(fixtureDef));

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
	sensorTop.SetAsBox(halfWidth - 0.1, 0.01, b2Vec2(0, -halfHeight + radius), 0);

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

	direction = RIGHT;
	grounded = false;

}

BadGuy::~BadGuy() {
	srand(time(NULL));
	for (map<std::string, int>::iterator it = items.begin(); it != items.end(); ++it) {
		for (int i = 0; i < it->second; i++) {
			new Item(it->first, body->GetPosition().x, body->GetPosition().y, (rand() % 20) - 10, -(rand() % 10));
		}
	}
}

void BadGuy::move() {

	if (grounded) {
		if (body->GetLinearVelocity().x < maxVelocity && direction == RIGHT) {
			body->ApplyLinearImpulse(b2Vec2(5, 0), body->GetWorldCenter());
			action = ACTION_WALK_RIGHT;
		} else if (body->GetLinearVelocity().x > -maxVelocity && direction == LEFT) {
			body->ApplyLinearImpulse(b2Vec2(-5, 0), body->GetWorldCenter());
			action = ACTION_WALK_LEFT;
		}
	}
}

void BadGuy::logic() {
	int collision = checkCollision();

	if (collision & UP) {
		alive = false;
		currentframe = 0;
		action = ACTION_DEAD;
		for (int i=0 ; i< wheels.size();i++){
			wheels[i]->SetFriction(20);
		}
	} else {
		if (collision & RIGHT) {
			direction = LEFT;
		} else if (collision & LEFT) {
			direction = RIGHT;
		}

		Entity::logic();
	}
}
