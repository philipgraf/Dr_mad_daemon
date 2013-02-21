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
#include <map>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <Box2D/Box2D.h>

//TODO find a better way
#define ACTION_STAY 0
#define ACTION_WALK_LEFT 1
#define ACTION_WALK_RIGHT 2
#define ACTION_JUMP_LEFT  3
#define ACTION_JUMP_RIGHT 4
#define ACTION_DUCK_LEFT 5
#define ACTION_DUCK_RIGHT 6
#define ACTION_DEAD 3

#define UP 0x01
#define RIGHT 0x02
#define DOWN 0x04
#define LEFT 0x08

/**
 * Entity Class all things you can interact with.
 */
class Entity {
protected:
	std::map<std::string, int> items;
	SDL_Surface * image; /**< Image of the entity with all animationframes */
	bool alive; /**< contains true if the entity is still alive and false if not */

	Uint8 direction;

	int currentframe; /**< Current Frame number this is used to calculate the correct part of the image */
	int action; /**< the action e.g. move left, move right, jump, needed for rendering  */
	//TODO: actionframes get from File ?!?
	std::vector<int> actionframes;
	std::vector<int> animationDuration;

	float width;/**< width of the entity in meter */
	float height;/**< height of the entity in meter */

	float maxVelocity; /**< the maximal velocity of the entity. May be manipulated for running, etc. **/

	bool grounded; /**< is true when sensorBottom collide with something. */

	b2Fixture *sensorRight;
	b2Fixture *sensorLeft;
	b2Fixture *sensorTop;
	b2Fixture *sensorBottom;

	b2Body *body;

	void nextframe();

public:
	static std::vector<Entity*> entityList;

	Entity();
	virtual ~Entity();

	virtual void logic();
	virtual void move();

	int checkCollision();

	//----------------------------------- Getter and Setter ------------------------------

	SDL_Rect getCurFrameRect();
	bool isAlive() const;
	void setAlive(bool alive);
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

	void setDirection(Uint8 direction);
	void delDirection(Uint8 direction);
	Uint8 getDirection() const;
	std::map<std::string, int>& getItems();
};

#endif /* ENTITY_H_ */
