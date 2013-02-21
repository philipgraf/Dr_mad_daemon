/*
 * Player.cpp
 *
 *  Created on: 12.02.2013
 *      Author: philip
 */

#include "Player.h"
#include "Game.h"
#include "Slot.h"
#include "BadGuy.h"
#include "Notification.h"

#include <sstream>

using namespace std;

Player::Player(int x, int y) :Entity() {

	width = 1;
	height = 2;

	actionframes.push_back(1);
	actionframes.push_back(4);
	actionframes.push_back(4);

	animationDuration.push_back(9999);
	animationDuration.push_back(60);
	animationDuration.push_back(60);

	float halfWidth = width / 2;
	float halfHeight = height / 2;

	maxVelocity = 5;

	running = false;

	impactSoundPlayed = 0;

	items = Slot::slots[Game::curGame->settings.activeSlot]->getPlayerItems();

	SDL_Surface *tmp = SDL_LoadBMP(IMG"player.bmp");

	if (!tmp) {
		//TODO Throw DISException
		cout << "unable to load BMP file player.bmp" << endl;
	} else {
		image = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		if (image != 0) {
			SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(image->format, 255, 0, 255));

		}
	}

	direction = 0;

	float radius = halfWidth;

	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.fixedRotation = true;
	bodydef.position.Set(x + halfWidth, y + halfHeight);
	this->body = Game::curGame->getCurrentLevel()->getWorld()->CreateBody(&bodydef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(halfWidth, halfHeight - radius / 2);

	b2FixtureDef *fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &dynamicBox;
	fixtureDef->density = 40.0f;
	fixtureDef->friction = 0.0f;
	body->CreateFixture(fixtureDef);

	b2CircleShape feetShape;
	feetShape.m_radius = radius;
	feetShape.m_p = b2Vec2(0, halfWidth + radius / 2);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &feetShape;
	fixtureDef->density = 40.0f;
	fixtureDef->friction = 0.2;

	feetFixture = body->CreateFixture(fixtureDef);

	b2PolygonShape sensorRight;
	sensorRight.SetAsBox(0.01, halfHeight - radius / 2 - 0.1, b2Vec2(halfWidth, 0), 0);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &sensorRight;
	fixtureDef->isSensor = true;
	this->sensorRight = body->CreateFixture(fixtureDef);

	b2PolygonShape sensorLeft;
	sensorLeft.SetAsBox(0.01, halfHeight - radius / 2 - 0.1, b2Vec2(-halfWidth, 0), 0);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &sensorLeft;
	fixtureDef->isSensor = true;
	this->sensorLeft = body->CreateFixture(fixtureDef);

	b2PolygonShape sensorTop;
	sensorTop.SetAsBox(halfWidth - 0.1, 0.01, b2Vec2(0, -halfHeight + radius / 2), 0);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &sensorTop;
	fixtureDef->isSensor = true;
	this->sensorTop = body->CreateFixture(fixtureDef);

	b2CircleShape sensorBottom;
	sensorBottom.m_radius = radius - 0.05;
	sensorBottom.m_p = b2Vec2(0, halfWidth + radius / 2 + 0.05);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &sensorBottom;
	fixtureDef->isSensor = true;
	this->sensorBottom = body->CreateFixture(fixtureDef);

}

Player::~Player() {
}

void Player::use() {
	BadGuy *badguy;
	// check player contacts with dead bodys and loot them
	for (b2ContactEdge *contactEdge = body->GetContactList(); contactEdge; contactEdge = contactEdge->next) {
		if (contactEdge->contact->GetFixtureA() == sensorBottom && contactEdge->contact->IsTouching()) {
			if ((badguy = (BadGuy*) contactEdge->contact->GetFixtureB()->GetBody()->GetUserData()) != NULL) {
				for(map<string,int>::iterator it = badguy->getItems().begin(); it != badguy->getItems().end();++it){
					std::stringstream s;
					s << lang["looted"] << " " << lang[it->first] << ": " << it->second;
					new Notification(s.str(),5,NOTIFICATION_INFO,it->first);
					items[it->first] += it->second;
				}
				delete badguy;
				break;
			}
		} else if (contactEdge->contact->GetFixtureB() == sensorBottom && contactEdge->contact->IsTouching()) {
			if ((badguy = (BadGuy*) contactEdge->contact->GetFixtureA()->GetBody()->GetUserData()) != NULL) {
				for(map<string,int>::iterator it = badguy->getItems().begin(); it != badguy->getItems().end();++it){
					std::stringstream s;
					s << lang["looted"] << " " << lang[it->first] << ": " << it->second;
					new Notification(s.str(),5,NOTIFICATION_INFO,it->first);
					items[it->first] += it->second;
				}
				delete badguy;
				break;
			}
		}
	}

	// check player position an get the flags of the tiles there
	switch (Game::curGame->getCurrentLevel()->getTilelist()[1][(int) body->GetPosition().x][(int) body->GetPosition().y]->getFlags()) {

	// if flag is finishpoint
	case 2:
		Game::curGame->getCurrentLevel()->setFinished(true);
		break;
	default:
		cout << "nothing" << endl;
		break;
	}
}

void Player::logic() {
//TODO check connected tiles for shock and other game events
	int collision = checkCollision();

	if (grounded && impactSoundPlayed > 20) {
		Mix_PlayChannel(-1, Game::sounds["player jump impact"], 0);
	} else if (!grounded) {
		impactSoundPlayed++;
	}
	if (grounded) {
		impactSoundPlayed = 0;
	}

	Entity::logic();
}

void Player::move() {
	if (running) {
		maxVelocity = 8;
	} else {
		maxVelocity = 4;
	}

	if (direction & LEFT) {
		feetFixture->SetFriction(0.2);
		if (checkCollision() & DOWN) {
			if (body->GetLinearVelocity().x > -maxVelocity) {
				body->ApplyLinearImpulse(b2Vec2(-body->GetMass() / 2, 0), body->GetWorldCenter());
			}
		} else {
			if (body->GetLinearVelocity().x > -maxVelocity) {
				body->ApplyLinearImpulse(b2Vec2(-body->GetMass() / 8, 0), body->GetWorldCenter());
			}
		}

		action = ACTION_WALK_LEFT;

	}
	if (direction & RIGHT) {
		feetFixture->SetFriction(0.2);
		if (checkCollision() & DOWN) {
			if (body->GetLinearVelocity().x < maxVelocity) {
				body->ApplyLinearImpulse(b2Vec2(body->GetMass() / 2, 0), body->GetWorldCenter());
			}
		} else {
			if (body->GetLinearVelocity().x < maxVelocity) {
				body->ApplyLinearImpulse(b2Vec2(body->GetMass() / 8, 0), body->GetWorldCenter());
			}
		}
		action = ACTION_WALK_RIGHT;

	}
	if (direction & UP) {
		if (checkCollision() & DOWN) {
			float impulse = body->GetMass();
			body->ApplyLinearImpulse(b2Vec2(0, -impulse * 4), body->GetWorldCenter());
			Mix_PlayChannel(-1, Game::sounds["player jump"], 0);
		}
		//action = ACTION_JUMP_LEFT;
	}

	if (!(direction & (LEFT | RIGHT | UP))) {
		feetFixture->SetFriction(200.0);
		action = ACTION_STAY;
	}

}

