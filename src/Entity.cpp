/** Entity.cpp
 *
 *  Created on: 27.09.2012
 *      Author: medi
 */

//#include <algorithm>
#include "Entity.h"

#include "define.h"
#include "Item.h"
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
Entity::Entity() {

	image = NULL;
	body = NULL;

	sensorBottom = NULL;
	sensorLeft = NULL;
	sensorRight = NULL;
	sensorTop = NULL;

	alive = true;
	currentframe = 0;
	action = ACTION_STAY;
	direction = 0;

	grounded = false;

	width = 0;
	height = 0;
	maxVelocity = 0;

	timer =  SDL_GetTicks();

	entityList.push_back(this);
}

/**
 * currently just call the move and nextframe function
 * @see move()
 * @see nextframe()
 */
void Entity::logic() {
	move();
	nextframe();
}

/** Changes the frame of a entity.
 * If the duration the current frame should be displayed is over it will be set to the next or the first (if the current frame is the last)
 *
 * @see actionframes
 * @see animationDuration
 */
void Entity::nextframe() {

	if (SDL_GetTicks() - timer > animationDuration[action]) {
		currentframe++;
		timer = SDL_GetTicks();
	}
	if (currentframe >= actionframes[action]) {
		currentframe = 0;
	}
}

/**
 * check the collision and set the corresponding bit of the return value and set grounded of true if the bottom sensors is touched.
 * 1st Bit for top
 * 2nd Bit for left
 * 3rd Bit for bottom
 * 4th Bit for right
 *
 * @see grounded
 * @return the collided sites as bits in a integer
 */
int Entity::checkCollision() {
	int retValue = 0;
	for (b2ContactEdge *contactEdge = body->GetContactList(); contactEdge; contactEdge = contactEdge->next) {
		if ((contactEdge->contact->GetFixtureA() == sensorRight || contactEdge->contact->GetFixtureB() == sensorRight) && contactEdge->contact->IsTouching()) {
			retValue |= RIGHT;
		} else if ((contactEdge->contact->GetFixtureA() == sensorLeft || contactEdge->contact->GetFixtureB() == sensorLeft) && contactEdge->contact->IsTouching()) {
			retValue |= LEFT;
		} else if ((contactEdge->contact->GetFixtureA() == sensorTop || contactEdge->contact->GetFixtureB() == sensorTop) && contactEdge->contact->IsTouching()) {
			retValue |= UP;
		} else if ((contactEdge->contact->GetFixtureA() == sensorBottom || contactEdge->contact->GetFixtureB() == sensorBottom) && contactEdge->contact->IsTouching()) {
			retValue |= DOWN;
		}
		// recalculate friction of contact
		contactEdge->contact->ResetFriction();
	}

	if (retValue & DOWN) {
		grounded = true;
	} else {
		grounded = false;
	}

	return retValue;
}

/**
 * virtual function
 * movement defined from inheriting classes
 */
void Entity::move() {
}

/**
 * Iterate thru the items and create for each a new item with random relX and relY values.
 * delete the entity in the entity list and free all allocated memory.
 * @see Item()
 * @see entityList()
 */
Entity::~Entity() {
	srand(time(NULL));
	for (map<std::string, int>::iterator it = items.begin(); it != items.end(); ++it) {
		for (int i = 0; i < it->second; i++) {
			new Item(it->first, body->GetPosition().x, body->GetPosition().y, (rand() % 20) - 10, -(rand() % 10));
		}
	}
	SDL_FreeSurface(image);
	std::vector<Entity*>::iterator pos;
	if ((pos = std::find(entityList.begin(), entityList.end(), this)) != entityList.end()) {
		entityList.erase(pos);
	}
	Game::curGame->getCurrentLevel()->getWorld()->DestroyBody(body);
	actionframes.clear();
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

/**
 * returns TRUE if the entity is alive
 * @return the alive state of the entity
 */
bool Entity::isAlive() const {
	return alive;
}

/**
 * set the alive state to the given value
 * @param alive new alive state of the entity
 */
void Entity::setAlive(bool alive) {
	this->alive = alive;
}

/**
 * get the x position of the entity
 * @return the x position of the entity
 */
float Entity::getX() const {
	return body->GetPosition().x;
}

/** Get get current y Position of the Entity.
 * @return the y position of the Entity
 */
float Entity::getY() const {
	return body->GetWorldCenter().y ;
}

/**
 * Get the image of the Entity
 * @return the image of the Entity as SDL_Surface
 */
SDL_Surface* Entity::getImage() {
	return image;
}

/**
 * get the current frame
 * @return the current frame
 */
int Entity::getCurrentframe() const {
	return currentframe;
}

/**
 * set the current frame to given value
 * @param the value current frame will be set to
 */
void Entity::setCurrentframe(int currentframe) {
	this->currentframe = currentframe;
}

/**
 * get the height of the Entity
 * @return the height of the Entity
 */
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

Uint8 Entity::getDirection() const {
	return direction;
}

void Entity::setDirection(Uint8 direction) {
	this->direction |= direction;
}

void Entity::delDirection(Uint8 direction) {
	this->direction &= ~direction;
}

std::map<std::string, int>& Entity::getItems() {
	return items;
}

void Entity::addItem(std::string item) {
	items[item]++;
}
