/*
 * Entity.h
 *
 *  Created on: 27.09.2012
 *      Author: medi
 */

#include "includes.h"

#ifndef ENTITY_H_
#define ENTITY_H_

#define UP 0x01
#define RIGHT 0x02
#define DOWN 0x04
#define LEFT 0x08

#define ACTION_STAY 0
#define ACTION_WALK_LEFT 1
#define ACTION_WALK_RIGHT 2
#define ACTION_JUMP_LEFT  3
#define ACTION_JUMP_RIGHT 4
#define ACTION_DUCK_LEFT 5
#define ACTION_DUCK_RIGHT 6


using namespace std;


/**
 * Entity Class all things you can interact with.
 */
class Entity {
private:
	SDL_Surface * image; /**< Image of the entity with all animationframes */
	bool alive; /**< contain true if the entity is still alive and false if not */
	int currentframe; /**< Current Frame number this is used to calculate the correct part of the image */
	int action; /**< the action e.g. move left, move right, jump, needed for rendering  */
	//TODO: actionframes get from File ?!?


	int flags;
	int x;
	int y;
	int width;
	int height;

	Uint8 direction;
	float speedX;
	float speedY;
	float maxSpeedX;
	float maxSpeedY;
	float accelX;
	float accelY;
	float breakX;

	void nextframe();



public:
	static vector<Entity*> entityList;

	Entity();
	Entity(string imagename,int w, int h);
	virtual ~Entity();

	void move();



	//----------------------------------- Getter and Setter ------------------------------


	/**
	 * Return the current animation picture of the entity
	 * @return the current animation picture as SDL_Rect
	 */
	SDL_Rect getCurFrameRect(){
		SDL_Rect rect;
		rect.x = currentframe*width;
		rect.y = action*height; // TODO: Animation
		rect.h = height;
		rect.w = width;
		return rect;
	}

	/**
	 * return the acceleration
	 */
	float getAccelX() const {
		return accelX;
	}

	void setAccelX(float accelX) {
		this->accelX = accelX;
	}

	float getAccelY() const {
		return accelY;
	}

	void setAccelY(float accelY) {
		this->accelY = accelY;
	}

	bool isAlive() const {
		return alive;
	}

	void setAlive(bool alive) {
		this->alive = alive;
	}

	float getBreakX() const {
		return breakX;
	}

	void setBreakX(float breakX) {
		this->breakX = breakX;
	}

	Uint8 getDirection() const {
		return direction;
	}


	void setDirection(Uint8 direction) {
		this->direction |= direction;
	}

	void  delDirection(Uint8 direction) {
		this->direction &= ~direction;
	}


	int getFlags() const {
		return flags;
	}

	void setFlags(int flags) {
		this->flags = flags;
	}

	float getMaxSpeedX() const {
		return maxSpeedX;
	}

	void setMaxSpeedX(float maxSpeedX) {
		this->maxSpeedX = maxSpeedX;
	}

	float getMaxSpeedY() const {
		return maxSpeedY;
	}

	void setMaxSpeedY(float maxSpeedY) {
		this->maxSpeedY = maxSpeedY;
	}

	float getSpeedX() const {
		return speedX;
	}

	void setSpeedX(float speedX) {
		this->speedX = speedX;
	}

	float getSpeedY() const {
		return speedY;
	}

	void setSpeedY(float speedY) {
		this->speedY = speedY;
	}

	int getX() const {
		return x;
	}

	void setX(int x) {
		this->x = x;
	}

	int getY() const {
		return y;
	}

	void setY(int y) {
		this->y = y;
	}

	SDL_Surface* getImage() {
		return image;
	}

	int getCurrentframe() const {
		return currentframe;
	}

	void setCurrentframe(int currentframe) {
		this->currentframe = currentframe;
	}

	int getHeight() const {
		return height;
	}

	void setHeight(int height) {
		this->height = height;
	}

	int getWidth() const {
		return width;
	}

	void setWidth(int width) {
		this->width = width;
	}
};

#endif /* ENTITY_H_ */
