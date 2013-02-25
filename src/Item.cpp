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
	fixDef->friction = 0.5;
	fixDef->restitution = 0.1;
	fixDef->density = 5;
	fixDef->filter.categoryBits = 1;
	fixDef->filter.maskBits =1;

	body->CreateFixture(fixDef);


	body->ApplyLinearImpulse(b2Vec2(relX, relY), body->GetWorldCenter());

	timer = SDL_GetTicks();

	Item::itemlist.push_back(this);

}

Item::~Item() {
	SDL_FreeSurface(image);
	vector<Item*>::iterator pos;
	if ((pos = find(itemlist.begin(), itemlist.end(), this)) != itemlist.end()) {
		itemlist.erase(pos);
	}
	Game::curGame->getCurrentLevel()->getWorld()->DestroyBody(body);
}

void Item::logic() {
	if(SDL_GetTicks() - timer > 1000){
		body->GetFixtureList()->SetFilterData(b2Filter());
	}
	for (b2ContactEdge *contactEdge = body->GetContactList(); contactEdge; contactEdge = contactEdge->next) {
		if (contactEdge->contact->GetFixtureA()->GetBody()->GetUserData() != NULL) {
			((Entity*) contactEdge->contact->GetFixtureA()->GetBody()->GetUserData())->addItem(type);
			//it's OK for an object to commit suicide
			delete this;
			break;
		} else if (contactEdge->contact->GetFixtureB()->GetBody()->GetUserData() != NULL) {
			((Entity*) contactEdge->contact->GetFixtureB()->GetBody()->GetUserData())->addItem(type);
			//it's OK for an object to commit suicide
			delete this;
			break;
		}
	}
}

std::string Item::getType() {
	return type;
}

SDL_Surface *Item::getImage() {
	return image;
}

SDL_Rect Item::getClipRect() {
	SDL_Rect retRect;
	retRect.w = image->clip_rect.w;
	retRect.h = image->clip_rect.h;
	retRect.x = body->GetPosition().x * TILESIZE - retRect.w / 2;
	retRect.y = body->GetPosition().y * TILESIZE - retRect.h / 2;
	return retRect;
}
