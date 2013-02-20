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
#include <Box2D/Box2D.h>


class Player: public Entity {
private:

	bool running;
	int impactSoundPlayed;
public:
	Player(int x, int y);
	virtual ~Player();

	b2Fixture *feetFixture;
	PDA pda;

	void logic();
	void move();
	void use();



	bool isRunning() const {
		return running;
	}

	void setRunning(bool running) {
		this->running = running;
	}
};

#endif /* PLAYER_H_ */
