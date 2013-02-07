/** Entity.cpp
 *
 *  Created on: 27.09.2012
 *      Author: medi
 */

//#include <algorithm>
#include "Entity.h"

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

	SDL_Surface *tmp = SDL_LoadBMP((IMG+imagename).c_str());

	if (!tmp) {
		//TODO Throw DISException
		cout << "unable to load BMP file" << imagename << endl;
	} else {
		image = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		if (image != 0) {
			SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL,
					SDL_MapRGB(image->format, 255, 0, 255));

		}
	}

	alive = true;
	flags = 0;
	currentframe = 0;
	action = ACTION_STAY;
	direction = 0;
	width = w;
	height = h;

	float radius =  w/2;

	ground = true; //TODO test only

	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.fixedRotation = true;
	bodydef.position.Set(x + w / 2, y + h / 2);
	this->body = Game::curGame->getCurrentLevel()->getWorld()->CreateBody(
			&bodydef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(w / 2-0.1, h / 2 - radius/2);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &dynamicBox;
	fixtureDef->density = 35.0f;
	fixtureDef->friction = 5.0f;

	body->CreateFixture(fixtureDef);

	b2BodyDef feetdef;
	feetdef.type = b2_dynamicBody;
	//bodydef.fixedRotation = true;
	feetdef.position.Set(x + w / 2, y + h -radius);
	this->feet = Game::curGame->getCurrentLevel()->getWorld()->CreateBody(
			&feetdef);
	b2CircleShape feetShape;
	feetShape.m_radius=radius;

	feetFixture = new b2FixtureDef;
	feetFixture->shape = &feetShape;
	feetFixture->density = 10.0f;
	feetFixture->friction = 5.0f;

	feet->CreateFixture(feetFixture);

	b2RevoluteJointDef joinDef;
	joinDef.Initialize(body,feet,body->GetWorldCenter());
	joinDef.localAnchorA=b2Vec2(0,h/2-radius);
	joinDef.localAnchorB=b2Vec2(0,0);
	joinDef.enableLimit=true;


	joint =(b2RevoluteJoint*) Game::curGame->getCurrentLevel()->getWorld()->CreateJoint(&joinDef);


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

		body->ApplyLinearImpulse(b2Vec2(-body->GetMass() / 2, 0),
				body->GetWorldCenter());
		action = ACTION_WALK_LEFT;

	}
	if (direction & RIGHT) {
		//b2Vec2 vel = body->GetLinearVelocity();

		feet->ApplyLinearImpulse(b2Vec2(body->GetMass() / 2, 0),
				body->GetWorldCenter());
		action = ACTION_WALK_RIGHT;

	}
	if (direction & UP) {
//		b2Vec2 vel = body->GetLinearVelocity();
//		vel.y = -7;
//		body->SetLinearVelocity(vel);
		//	body->ApplyForce(b2Vec2(0.0f,-500.0f),body->GetWorldCenter());
		if (feet->GetContactList() != NULL) {
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

/**
 * Return the current animation picture of the entity
 * @return the current animation picture as SDL_Rect
 */
SDL_Rect Entity::getCurFrameRect() {
	SDL_Rect rect;
	rect.x = currentframe * width * TILESIZE;
	rect.y = action * height * TILESIZE; // TODO: Animation
	rect.h = height * TILESIZE;
	rect.w = width * TILESIZE;
	return rect;
}

bool Entity::isAlive() const {
	return alive;
}

void Entity::setAlive(bool alive) {
	this->alive = alive;
}

Uint8 Entity::getDirection() const {
	return direction;
}

void Entity::setDirection(Uint8 direction) {
	this->direction |= direction;
}

void Entity::delDirection(Uint8 direction) {
	this->direction &= ~direction;
}

int Entity::getFlags() const {
	return flags;
}

void Entity::setFlags(int flags) {
	this->flags = flags;
}

//TODO replace
float Entity::getX() const {
	return body->GetPosition().x;
}

float Entity::getY() const {
	return body->GetPosition().y;
}

SDL_Surface* Entity::getImage() {
	return image;
}

int Entity::getCurrentframe() const {
	return currentframe;
}

void Entity::setCurrentframe(int currentframe) {
	this->currentframe = currentframe;
}

float Entity::getHeight() const {
	return height;
}

void Entity::setHeight(float height) {
	this->height = height;
}

float Entity::getWidth() const {
	return width;
}

void Entity::setWidth(float width) {
	this->width = width;
}

b2Body* Entity::getBody() {
	return body;
}
