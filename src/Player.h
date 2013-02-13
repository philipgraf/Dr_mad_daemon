/*
 * Player.h
 *
 *  Created on: 12.02.2013
 *      Author: philip
 */


#ifndef PLAYER_H_
#define PLAYER_H_


#define UP 0x01
#define RIGHT 0x02
#define DOWN 0x04
#define LEFT 0x08


#include "Entity.h"


class Player: public Entity {
private:
	bool use;
	Uint8 direction;
public:
	Player(int x, int y);
	virtual ~Player();

	void logic();

	void move();

	bool isUse() const;
	void setUse(bool use);
	Uint8 getDirection() const;
	void setDirection(Uint8 direction);
	void delDirection(Uint8 direction);
};

#endif /* PLAYER_H_ */
