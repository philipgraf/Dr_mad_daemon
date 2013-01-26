/** Entity.cpp
 *
 *  Created on: 27.09.2012
 *      Author: medi
 */

#include "Entity.h"
#include <algorithm>

vector<Entity*> Entity::entityList;
//TODO: get from file and save NOT static in Entity class
int actionframes[] = {1,4,4};

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
 */
Entity::Entity(string imagename, int w,int h) {
	image = Tools::loadImage(imagename);
	alive = true;
	flags = 0;
	currentframe=0;
	action=ACTION_STAY;
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

void Entity::nextframe() {
	currentframe++;
	if(currentframe >= actionframes[action]){
		currentframe=0;
	}
}

// FIXME better move algorithm needed
/**
 * handle player movement
 * \todo better move algorithm needed
 */
void Entity::move() {
	if ((direction & LEFT) && -maxSpeedX < speedX) {
		speedX -= accelX;
		action = ACTION_WALK_LEFT;

	} else if ((direction & RIGHT) && maxSpeedX > speedX) {
		speedX += accelX;
		action = ACTION_WALK_RIGHT;
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

	if(speedX>-1 && speedX<1){
		speedX=0;
		action=ACTION_STAY;
	}

	if(speedY>-1 && speedY<1){
		speedY=0;
	}

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
	nextframe();

}

Entity::~Entity() {
	SDL_FreeSurface(image);
	std::vector<Entity*>::iterator pos;
	if(( pos = std::find(entityList.begin(),entityList.end(),this))!=entityList.end()){
		entityList.erase(pos);
	}
}


