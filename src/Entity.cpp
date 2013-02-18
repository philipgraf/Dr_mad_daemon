/** Entity.cpp
 *
 *  Created on: 27.09.2012
 *      Author: medi
 */

//#include <algorithm>
#include "Entity.h"

#include "define.h"
#include "Game.h"

vector<Entity*> Entity::entityList;


/** Constuctor
 * Set the given values an initial all other Entityvalues with default values
 * @param imagename Name of the image which will be loaded and converted into a SDL_Surface
 * @param w The width of the entity
 * @param h The height of the entity
 * @param x The x value of the entity
 * @param y the y value of the entity
 */
Entity::Entity(int numOfActions) {

	image = NULL;
	body=NULL;
	actionframes = new int[numOfActions];

	alive = true;
	currentframe = 0;
	action = ACTION_STAY;

	width = 0;
	height = 0;
	grounded = false;
	maxVelocity=0;

	entityList.push_back(this);
}


void Entity::logic() {
	move();
	nextframe();
}

void Entity::nextframe() {
	currentframe++;
	if (currentframe >= actionframes[action]) {
		currentframe = 0;
	}
}



// FIXME better move algorithm needed
/**
 * virtual function
 * movement defined from inheriting classes
 *
 */
void Entity::move() {
}

Entity::~Entity() {
	SDL_FreeSurface(image);
	std::vector<Entity*>::iterator pos;
	if ((pos = std::find(entityList.begin(), entityList.end(), this))
			!= entityList.end()) {
		entityList.erase(pos);
	}
	delete [] actionframes;
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
