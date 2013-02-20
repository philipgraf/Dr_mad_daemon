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
#include <map>
#include <string>


class Player: public Entity {
private:
	map<std::string,int> items;
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

	std::map<std::string,int>& getItems();
};

#endif /* PLAYER_H_ */
