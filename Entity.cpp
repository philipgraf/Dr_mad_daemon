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
Entity::Entity(string imagename, int w,int h) {
	image = Tools::loadImage(imagename);
	alive = true;
	flags = 0;
	currentframe=0;
	direction = 0;
	this->x = x;
	this->y = y;
	width = w;
	height = h;
	accelX = 1.1;
	accelY = 1.1;
	breakX = 1.5;
	maxSpeedX = (float) TILESIZE/4;
	maxSpeedY = (float) TILESIZE/4;
	speedX = 0.0;
	speedY = 0.0;
	entityList.push_back(this);
}

// FIXME better move algorithm needed
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
		speedX /= accelX;
	} else if (!(direction & RIGHT) && speedX > 0) {
		speedX /= accelX;
	}

	if (!(direction & UP) && speedY < 0) {
		speedY /= accelY;
	} else if (!(direction & DOWN) && speedY > 0) {
		speedY /= accelY;
	}

	if(speedX>-1 && speedX<1)
		speedX=0;

	if(speedY>-1 && speedY<1)
		speedY=0;

	x +=(int)speedX;
	y +=(int)speedY;

	if(x<0){
		x=0;
		speedX=0;
	}
	else if(x>(Game::curGame->getCurrentLevel()->getWidth())*TILESIZE-width){
		x=(Game::curGame->getCurrentLevel()->getWidth())*TILESIZE-width;
		speedX=0;
	}

}

Entity::~Entity() {
}


void Entity::render() {

}

