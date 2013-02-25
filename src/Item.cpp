/*
 * Item.cpp
 *
 *  Created on: 25.02.2013
 *      Author: philip
 */

#include "Item.h"
#include "define.h"
#include "Game.h"

using namespace std;

vector<Item*> Item::itemlist;

Item::Item(std::string name, int x, int y, int relX, int relY) {

	this->type = name;

	SDL_Surface *tmp = SDL_LoadBMP((IMG+name+".bmp").c_str());
	if (tmp != NULL) {
		image = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		if (image != 0) {
			SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(image->format, 255, 0, 255));

		}
	}

	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.fixedRotation = true;
	bodydef.position = b2Vec2(x, y);

	this->body = Game::curGame->getCurrentLevel()->getWorld()->CreateBody(&bodydef);

	b2PolygonShape polyshape;
	polyshape.SetAsBox(0.3125, 0.3125);

	b2FixtureDef *fixDef = new b2FixtureDef;
	fixDef->shape = &polyshape;
	fixDef->friction = 200;
	fixDef->restitution = 0.5;
	fixDef->density = 5;

	body->CreateFixture(fixDef);

	b2PolygonShape sensorShape;
	sensorShape.SetAsBox(0.3125, 0.32, b2Vec2(0.0, 0.01), 0);

	sensor = body->CreateFixture(&sensorShape, 0);

	body->ApplyLinearImpulse(b2Vec2(relX, relY), body->GetWorldCenter());

	Item::itemlist.push_back(this);

}

Item::~Item() {
	SDL_FreeSurface(image);
	std::vector<Item*>::iterator pos;
	if ((pos = std::find(itemlist.begin(), itemlist.end(), this)) != itemlist.end()) {
		itemlist.erase(pos);
	}
	Game::curGame->getCurrentLevel()->getWorld()->DestroyBody(body);
}

