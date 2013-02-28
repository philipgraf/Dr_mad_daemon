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

class Player: public Entity {
private:
	bool running;
	int impactSoundPlayed;
public:
	Player(int x, int y);
	virtual ~Player();

	PDA pda;

	Language lang;

	void logic();
	void move();
	void use();

	bool isRunning() const {
		return running;
	}

	void setRunning(bool running) {
		this->running = running;
	}
	void addItem(std::string item);

};

#endif /* PLAYER_H_ */
