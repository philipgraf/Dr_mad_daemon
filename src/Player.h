/*
 * Player.h
 *
 *  Created on: 12.02.2013
 *      Author: philip
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Entity.h"
#include "PDA.h"
#include "Language.h"
#include <Box2D/Box2D.h>
#include <map>
#include <string>
#include <cmath>

class Player: public Entity {
private:
	bool running;
	bool jumping;
	unsigned selectedEntity;
	int impactSoundPlayed;
	b2RevoluteJoint *grebJoin;
public:
	Player(int x, int y, int level);
	virtual ~Player();

	PDA pda;

	Language lang;


	void use();
	void move();
	void grab();
	void logic();

	bool isRunning() const {
		return running;
	}

	void setRunning(bool running) {
		this->running = running;
	}
	void addItem(std::string item);
	PDA &getpda();
	float getY() const;
	unsigned getSelectedEntity() const;
	bool isJumping() const;
	void setJumping(bool jumping);
};

b2Vec2 addAngle(b2Vec2 vector, float angle);
b2Vec2 setAngle(b2Vec2 vector, float angle);

#endif /* PLAYER_H_ */
