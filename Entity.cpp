/** Entity.cpp
 *
 *  Created on: 27.09.2012
 *      Author: medi
 */

#include "Entity.h"

vector<Entity*> Entity::entityList;

Entity::Entity() {
	// TODO Auto-generated constructor stub

}
Entity::Entity(string imagename, int x, int y) {
	image = Tools::loadImage(imagename);
	alive = true;
	flags = 0;
	direction = 0;
	this->x = x;
	this->y = y;
	accelX = 1.0;
	accelY = 1.0;
	maxSpeedX = (float) TILESIZE;
	maxSpeedY = (float) TILESIZE;
	speedX = 0.0;
	speedY = 0.0;
}

float Entity::getAccelX() const {
	return accelX;
}

void Entity::setAccelX(float accelX) {
	this->accelX = accelX;
}

float Entity::getAccelY() const {
	return accelY;
}

void Entity::setAccelY(float accelY) {
	this->accelY = accelY;
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

SDL_Surface* Entity::getImage() const {
	return image;
}

float Entity::getMaxSpeedX() const {
	return maxSpeedX;
}

void Entity::setMaxSpeedX(float maxSpeedX) {
	this->maxSpeedX = maxSpeedX;
}

float Entity::getMaxSpeedY() const {
	return maxSpeedY;
}

void Entity::setMaxSpeedY(float maxSpeedY) {
	this->maxSpeedY = maxSpeedY;
}

float Entity::getSpeedX() const {
	return speedX;
}

void Entity::setSpeedX(float speedX) {
	this->speedX = speedX;
}

float Entity::getSpeedY() const {
	return speedY;
}

void Entity::setSpeedY(float speedY) {
	this->speedY = speedY;
}

int Entity::getX() const {
	return x;
}

void Entity::setX(int x) {
	this->x = x;
}

int Entity::getY() const {
	return y;
}

void Entity::setY(int y) {
	this->y = y;
}

void Entity::move() {
	if ((direction & LEFT) && -maxSpeedX < speedX) {
		speedX -= accelX;
	} else if ((direction & RIGHT) && maxSpeedX > speedX) {
		speedX += accelX;

	}

	if ((direction & UP) && -maxSpeedY < speedY) {
		speedY -= accelY;
	} else if ((direction & DOWN) && maxSpeedY > speedY) {
		speedY += accelY;
	}

	if (!(direction & LEFT) && speedX < 0) {
		speedX += accelX;
	} else if (!(direction & RIGHT) && speedX > 0) {
		speedX -= accelX;
	}

	if (!(direction & UP) && speedY < 0) {
		speedY += accelY;
	} else if (!(direction & DOWN) && speedY > 0) {
		speedY -= accelY;
	}

	x += speedX;
	y += speedY;

	if(x<0){
		x=0;
		speedX=0;
	}
	else if(x>768){
		x=768;
		speedX=0;
	}

}

Entity::~Entity() {
	// TODO Auto-generated destructor stub
}

void Entity::render() {
	Tools::drawImage(SDL_GetVideoSurface(), x, y, image, 0, 0, 32, 64);
}

