/*
 * Entity.h
 *
 *  Created on: 27.09.2012
 *      Author: medi
 */
#ifndef ENTITY_H_
#define ENTITY_H_

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <Box2D/Box2D.h>


using namespace std;

/**
 * Entity Class all things you can interact with.
 */
class Entity {
protected:
	SDL_Surface * image; /**< Image of the entity with all animationframes */
	bool alive; /**< contain true if the entity is still alive and false if not */


	
	int currentframe; /**< Current Frame number this is used to calculate the correct part of the image */
	int action; /**< the action e.g. move left, move right, jump, needed for rendering  */
	//TODO: actionframes get from File ?!?


	float width;
	float height;

	bool ground; //TODO test only

	b2Body *body;
	b2FixtureDef *fixtureDef;

	b2Body *feet;
	b2FixtureDef *feetFixture;

	b2RevoluteJoint *joint;

	Uint8 direction;

	void nextframe();



public:
	static vector<Entity*> entityList;

	Entity(string imagename,float w, float h,int x, int y);
	virtual ~Entity();

	virtual void logic();
	virtual void move();



	//----------------------------------- Getter and Setter ------------------------------

	SDL_Rect getCurFrameRect();

	bool isAlive() const;

	void setAlive(bool alive);

	Uint8 getDirection() const;

	void setDirection(Uint8 direction);

	void  delDirection(Uint8 direction);

	float getX() const;

	float getY() const;

	SDL_Surface* getImage();

	int getCurrentframe() const;

	void setCurrentframe(int currentframe);

	float getHeight() const;

	void setHeight(float height);

	float getWidth() const;

	void setWidth(float width);

	b2Body* getBody();

};

#endif /* ENTITY_H_ */
