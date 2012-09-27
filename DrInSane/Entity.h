/*
 * Entity.h
 *
 *  Created on: 27.09.2012
 *      Author: medi
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#define UP 0x01
#define RIGHT 0x02
#define DOWN 0x04
#define LEFT 0x08

#include "define.h"
#include "Tools.h"
#include <SDL/SDL.h>
#include <string>
#include <vector>

using namespace std;


class Entity {
private:
	SDL_Surface * image;
	bool alive;
	int flags;
	int x;
	int y;
	Uint8 direction;
	float speedX;
	float speedY;
	float maxSpeedX;
	float maxSpeedY;
	float accelX;
	float accelY;

public:
	static vector<Entity*> entityList;
	Entity();
	Entity(string imagename,int x,int y);
	virtual ~Entity();
	float getAccelX() const;
	void setAccelX(float accelX);
	float getAccelY() const;
	void setAccelY(float accelY);
	bool isAlive() const;
	void setAlive(bool alive);
	Uint8 getDirection() const;
	void setDirection(Uint8 direction);
	void delDirection(Uint8 direction);
	int getFlags() const;
	void setFlags(int flags);
	SDL_Surface* getImage() const;
	float getMaxSpeedX() const;
	void setMaxSpeedX(float maxSpeedX);
	float getMaxSpeedY() const;
	void setMaxSpeedY(float maxSpeedY);
	float getSpeedX() const;
	void setSpeedX(float speedX);
	float getSpeedY() const;
	void setSpeedY(float speedY);
	int getX() const;
	void setX(int x);
	int getY() const;
	void setY(int y);

	void move();

	void render();
};

#endif /* ENTITY_H_ */
