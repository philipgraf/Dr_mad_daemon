/** Entity.cpp
 *
 *  Created on: 27.09.2012
 *      Author: medi
 */

#include "Entity.h"
#include <algorithm>

vector<Entity*> Entity::entityList;
//TODO: get from file and save NOT static in Entity class
int actionframes[] = { 1, 4, 4 };

/**
 * currently do nothing
 */
Entity::Entity() {
	// TODO Auto-generated constructor stub

}

/** Constuctor
 * Set the given values an initial all other Entityvalues with default values
 * @param imagename Name of the image which will be loaded and converted into a SDL_Surface
 * @param w The width of the entity
 * @param h The height of the entity
 * @param x The x value of the entity
 * @param y the y value of the entity
 */
Entity::Entity(string imagename, float w, float h, int x, int y) {
	image = Tools::loadImage(imagename);
	alive = true;
	flags = 0;
	currentframe = 0;
	action = ACTION_STAY;
	direction = 0;
	width = w;
	height = h;

	ground = true; //TODO test only

	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.fixedRotation = true;
	bodydef.position.Set(x + w / 2, y + h / 2);
	this->body = Game::curGame->getCurrentLevel()->getWorld()->CreateBody(
			&bodydef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(w / 2, h / 2);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &dynamicBox;
	fixtureDef->density = 35.0f;
	fixtureDef->friction = 1.0f;

	body->CreateFixture(fixtureDef);

	entityList.push_back(this);
}

void Entity::nextframe() {
	currentframe++;
	if (currentframe >= actionframes[action]) {
		currentframe = 0;
	}
}

// FIXME better move algorithm needed
/**
 * handle player movement
 * \todo better move algorithm needed
 */
void Entity::move() {

	if (direction & LEFT) {

		body->ApplyLinearImpulse(b2Vec2(-15, 0), body->GetWorldCenter());
		action = ACTION_WALK_LEFT;

	}
	if (direction & RIGHT) {
		b2Vec2 vel = body->GetLinearVelocity();

		body->ApplyLinearImpulse(b2Vec2(15, 0), body->GetWorldCenter());
		action = ACTION_WALK_RIGHT;

	}
	if (direction & UP) {
//		b2Vec2 vel = body->GetLinearVelocity();
//		vel.y = -7;
//		body->SetLinearVelocity(vel);
		//	body->ApplyForce(b2Vec2(0.0f,-500.0f),body->GetWorldCenter());
		if (body->GetContactList() != NULL) {
			float impulse = body->GetMass();
			body->ApplyLinearImpulse(b2Vec2(0, -impulse),
					body->GetWorldCenter());

		}
		//action = ACTION_JUMP_LEFT;
	}
	nextframe();

}

Entity::~Entity() {
	SDL_FreeSurface(image);
	std::vector<Entity*>::iterator pos;
	if ((pos = std::find(entityList.begin(), entityList.end(), this))
			!= entityList.end()) {
		entityList.erase(pos);
	}
	delete fixtureDef;
}

