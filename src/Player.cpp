/*
 * Player.cpp
 *
 *  Created on: 12.02.2013
 *      Author: philip
 */

#include "Player.h"
#include "Game.h"

Player::Player(int x, int y) :Entity() {

	width=1;
	height=2;

	SDL_Surface *tmp = SDL_LoadBMP(IMG"player.bmp");

	if (!tmp) {
		//TODO Throw DISException
		cout << "unable to load BMP file player.bmp" << endl;
	} else {
		image = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		if (image != 0) {
			SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL,
					SDL_MapRGB(image->format, 255, 0, 255));

		}
	}

	direction = 0;

	use = false;
	float radius =  width/2;

	b2BodyDef bodydef;
		bodydef.type = b2_dynamicBody;
		bodydef.fixedRotation = true;
		bodydef.position.Set(x + width / 2, y + height / 2);
		this->body = Game::curGame->getCurrentLevel()->getWorld()->CreateBody(
				&bodydef);
		b2PolygonShape dynamicBox;

		//dynamicBox.SetAsBox(width / 2, height / 2);
		b2Vec2 vertices[6];
		vertices[0].Set(width/2,-height/2);
		vertices[1].Set(width/2,height/2-0.2);
		vertices[2].Set(width/2-0.2,height/2);
		vertices[3].Set(-width/2+0.2,height/2);
		vertices[4].Set(-width/2,height/2-0.2);
		vertices[5].Set(-width/2,-height/2);
		dynamicBox.Set(vertices,6);

		b2FixtureDef *fixtureDef = new b2FixtureDef;
		fixtureDef->shape = &dynamicBox;
		fixtureDef->density = 35.0f;
		fixtureDef->friction = 5.0f;

		body->CreateFixture(fixtureDef);

//		b2BodyDef feetdef;
//		feetdef.type = b2_dynamicBody;
//		//bodydef.fixedRotation = true;
//		feetdef.position.Set(x + w / 2, y + h -radius);
//		this->feet = Game::curGame->getCurrentLevel()->getWorld()->CreateBody(
//				&feetdef);
//		b2CircleShape feetShape;
//		feetShape.m_radius=radius;
//
//		feetFixture = new b2FixtureDef;
//		feetFixture->shape = &feetShape;
//		feetFixture->density = 10.0f;
//		feetFixture->friction = 5.0f;
//
//		feet->CreateFixture(feetFixture);
//
//		b2RevoluteJointDef joinDef;
//		joinDef.Initialize(body,feet,body->GetWorldCenter());
//		joinDef.localAnchorA=b2Vec2(0,h/2-radius);
//		joinDef.localAnchorB=b2Vec2(0,0);
//		joinDef.enableLimit=true;
//
//
//		joint =(b2RevoluteJoint*) Game::curGame->getCurrentLevel()->getWorld()->CreateJoint(&joinDef);
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

		body->ApplyLinearImpulse(b2Vec2(body->GetMass() / 2, 0),
				body->GetWorldCenter());
		action = ACTION_WALK_RIGHT;

	}
	if (direction & UP) {
		//	body->ApplyForce(b2Vec2(0.0f,-500.0f),body->GetWorldCenter());
		if (body->GetContactList() != NULL) {
			float impulse = body->GetMass();
			body->ApplyLinearImpulse(b2Vec2(0, -impulse),
					body->GetWorldCenter());
			Mix_PlayChannel(-1, Game::sounds["player jump"], 0);

		}
		//action = ACTION_JUMP_LEFT;
	}

}


Uint8 Player::getDirection() const {
	return direction;
}

void Player::setDirection(Uint8 direction) {
	this->direction |= direction;
}

void Player::delDirection(Uint8 direction) {
	this->direction &= ~direction;
}
