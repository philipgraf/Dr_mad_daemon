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

#define ACTION_STAY 0
#define ACTION_WALK_LEFT 1
#define ACTION_WALK_RIGHT 2
#define ACTION_JUMP_LEFT  3
#define ACTION_JUMP_RIGHT 4
#define ACTION_DUCK_LEFT 5
#define ACTION_DUCK_RIGHT 6

#include "Entity.h"


class Player: public Entity {
private:
	bool use;
public:
	Player(string imagename,float w, float h,int x, int y);
	virtual ~Player();

	void logic();

	void move();

	bool isUse() const;
	void setUse(bool use);
};

#endif /* PLAYER_H_ */
