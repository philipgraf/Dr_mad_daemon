/*
 * BadGuy.cpp
 *
 *  Created on: 12.02.2013
 *      Author: philip
 */

#include "BadGuy.h"
#include "define.h"
#include <iostream>
#include <Box2D/Box2D.h>
#include <SDL/SDL.h>

#include "Game.h"

using namespace std;

BadGuy::BadGuy(string imagename, float w, float h, int x, int y) :
		Entity(3) {

	actionframes[ACTION_STAY]=1;
	actionframes[ACTION_WALK_RIGHT]=1;
	actionframes[ACTION_WALK_LEFT]=1;

	maxVelocity = 3.0;
	width = w;
	height = h;

	w /= 2;
	h /= 2;

	SDL_Surface *tmp = SDL_LoadBMP(IMG"badguy.bmp");

	if (!tmp) {
		//TODO Throw DISException
		cout << "unable to load BMP file player.bmp" << endl;
	} else {
		image = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		if (image != 0) {
			SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL,
					SDL_MapRGB(image->format, 255, 0, 255));
		}
	}
	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.fixedRotation = true;
	bodydef.position.Set(x + w, y + h);
	this->body = Game::curGame->getCurrentLevel()->getWorld()->CreateBody(
			&bodydef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(w, h);
	b2FixtureDef *fixureDef = new b2FixtureDef;
	fixureDef->shape = &dynamicBox;
	fixureDef->density = 35.0;
	fixureDef->friction = 0.3;
	body->CreateFixture(fixureDef);

}

BadGuy::~BadGuy() {
	// TODO Auto-generated destructor stub
}

void BadGuy::move() {
	if (body->GetLinearVelocity().y == 0) {
		if (body->GetLinearVelocity().x < maxVelocity) {
			body->ApplyLinearImpulse(b2Vec2(5, 0), body->GetWorldCenter());
			action=ACTION_WALK_RIGHT;
		}
	}
}

void BadGuy::logic() {
	Entity::logic();
}
