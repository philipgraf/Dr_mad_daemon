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



using namespace std;


class Entity {
private:
	SDL_Surface * image;
	bool alive;
	int currentframe;

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



public:
	static vector<Entity*> entityList;
	Entity();
	Entity(string imagename,int w, int h);
	virtual ~Entity();

	void move();

	void render();


	//----------------------------------- Getter and Setter ------------------------------

	SDL_Rect getCurFrameRect(){
		SDL_Rect rect;
		rect.x = currentframe*width;
		rect.y = 0; // TODO: Animation
		rect.h = height;
		rect.w = width;
		return rect;
	}

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
