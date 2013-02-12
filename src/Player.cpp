/*
 * Player.cpp
 *
 *  Created on: 12.02.2013
 *      Author: philip
 */

#include "Player.h"
#include "Game.h"

Player::Player(string imagename, float w, float h, int x, int y) :
		Entity(imagename, w, h, x, y) {
	use = false;
}

Player::~Player() {
	// TODO Auto-generated destructor stub
}

bool Player::isUse() const {
	return use;
}

void Player::logic() {

	if (use) {
		switch (Game::curGame->getCurrentLevel()->getTilelist()[1][(int) body->GetPosition().x][(int) body->GetPosition().y]->getFlags()) {
		case 2:
			Game::curGame->getCurrentLevel()->setFinished(true);
			break;
		default:
			cout << "nothing" << endl;
			break;
		}
		use = false;
	}

	Entity::logic();
}

void Player::setUse(bool use) {
	this->use = use;
}

void Player::move() {

	if (direction & LEFT) {

		body->ApplyLinearImpulse(b2Vec2(-body->GetMass() / 2, 0),
				body->GetWorldCenter());
		action = ACTION_WALK_LEFT;

	}
	if (direction & RIGHT) {
		//b2Vec2 vel = body->GetLinearVelocity();

		feet->ApplyLinearImpulse(b2Vec2(body->GetMass() / 2, 0),
				body->GetWorldCenter());
		action = ACTION_WALK_RIGHT;

	}
	if (direction & UP) {
//		b2Vec2 vel = body->GetLinearVelocity();
//		vel.y = -7;
//		body->SetLinearVelocity(vel);
		//	body->ApplyForce(b2Vec2(0.0f,-500.0f),body->GetWorldCenter());
		if (feet->GetContactList() != NULL) {
			float impulse = body->GetMass();
			body->ApplyLinearImpulse(b2Vec2(0, -impulse),
					body->GetWorldCenter());
			Mix_PlayChannel(-1, Game::sounds["player jump"], 0);

		}
		//action = ACTION_JUMP_LEFT;
	}

}
