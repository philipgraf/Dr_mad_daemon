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
	float width;
	float height;

	b2Body *body;
	b2FixtureDef *fixtureDef;

	Uint8 direction;

	void nextframe();



public:
	static vector<Entity*> entityList;

	Entity();
	Entity(string imagename,float w, float h,int x, int y);
	virtual ~Entity();

	void move();



	//----------------------------------- Getter and Setter ------------------------------


	/**
	 * Return the current animation picture of the entity
	 * @return the current animation picture as SDL_Rect
	 */
	SDL_Rect getCurFrameRect(){
		SDL_Rect rect;
		rect.x = currentframe*width*TILESIZE;
		rect.y = action*height*TILESIZE; // TODO: Animation
		rect.h = height*TILESIZE;
		rect.w = width*TILESIZE;
		return rect;
	}

	bool isAlive() const {
		return alive;
	}

	void setAlive(bool alive) {
		this->alive = alive;
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

	//TODO replace
	float getX() const {
		return body->GetPosition().x;
	}

	float getY() const {
		return body->GetPosition().y;
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

	b2Body* getBody()
	{
		return body;
	}
};

#endif /* ENTITY_H_ */
