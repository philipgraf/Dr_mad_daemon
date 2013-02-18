/*
 * Player.cpp
 *
 *  Created on: 12.02.2013
 *      Author: philip
 */

#include "Player.h"
#include "Game.h"

Player::Player(int x, int y) :
		Entity(3) {

	width = 1;
	height = 2;

	actionframes[ACTION_STAY]=1;
	actionframes[ACTION_WALK_RIGHT]=4;
	actionframes[ACTION_WALK_LEFT]=4;

	float halfWidth = width / 2;
	float halfHeight = height / 2;

	//TODO get from config
	maxVelocity = 5;

	running = false;

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

	float radius = halfWidth - 0.15;

	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.fixedRotation = true;
	bodydef.position.Set(x + halfWidth, y + halfHeight);
	this->body = Game::curGame->getCurrentLevel()->getWorld()->CreateBody(
			&bodydef);
	b2PolygonShape dynamicBox;

	//dynamicBox.SetAsBox(width / 2, height / 2);
	b2Vec2 vertices[6];
	vertices[0].Set(halfWidth, -halfHeight);
	vertices[1].Set(halfWidth, 0);
	vertices[2].Set(halfWidth - 0.2, halfHeight - radius);
	vertices[3].Set(-(halfWidth - 0.2), halfHeight - radius);
	vertices[4].Set(-halfWidth, 0);
	vertices[5].Set(-halfWidth, -halfHeight);
	dynamicBox.Set(vertices, 6);

	b2FixtureDef *fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &dynamicBox;
	fixtureDef->density = 40.0f;
	fixtureDef->friction = 0.0f;
	body->CreateFixture(fixtureDef);

	b2CircleShape feetShape;
	feetShape.m_radius = radius;
	feetShape.m_p = b2Vec2(0, halfWidth + radius - 0.2);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &feetShape;
	fixtureDef->density = 40.0f;
	fixtureDef->friction = 0.2;

	feetFixture = body->CreateFixture(fixtureDef);
}

Player::~Player() {
	// TODO Auto-generated destructor stub
}

void Player::use() {

	switch (Game::curGame->getCurrentLevel()->getTilelist()[1][(int) body->GetPosition().x][(int) body->GetPosition().y]->getFlags()) {
	case 2:
		Game::curGame->getCurrentLevel()->setFinished(true);
		break;
	default:
		cout << "nothing" << endl;
		break;
	}
}

void Player::logic() {
//TODO check connected tiles for shock and other game events
	int contacts = 0;
	for (b2ContactEdge *contactEdge = body->GetContactList(); contactEdge;
			contactEdge = contactEdge->next) {
		b2WorldManifold worldMani;
		contactEdge->contact->GetWorldManifold(&worldMani);
		double collideY = worldMani.points[0].y - body->GetPosition().y;
		if (collideY <= 1.0 && collideY >= 0.9) {
			contacts++;
		}
		// recalculate friction of contact
		contactEdge->contact->ResetFriction();
	}

	if (contacts) {
		grounded = true;

	} else {
		grounded = false;
	}

	Entity::logic();
}

void Player::move() {
	if (running) {
		maxVelocity = 8;
	}else{
		maxVelocity = 4;
	}

	if (direction & LEFT) {
		if (grounded) {
			feetFixture->SetFriction(0.2);
			if (body->GetLinearVelocity().x > -maxVelocity) {
				body->ApplyLinearImpulse(b2Vec2(-body->GetMass()/2, 0),
						body->GetWorldCenter());
			}
		} else {
			if (body->GetLinearVelocity().x > -maxVelocity) {
				body->ApplyLinearImpulse(b2Vec2(-body->GetMass()/8, 0), body->GetWorldCenter());
			}
		}

		action = ACTION_WALK_LEFT;

	}
	if (direction & RIGHT) {
		if (grounded) {
			feetFixture->SetFriction(0.2);
			if (body->GetLinearVelocity().x < maxVelocity) {
				body->ApplyLinearImpulse(b2Vec2(body->GetMass()/2, 0),
						body->GetWorldCenter());
			}
		} else {
			if (body->GetLinearVelocity().x < maxVelocity) {
				body->ApplyLinearImpulse(b2Vec2(body->GetMass()/8, 0), body->GetWorldCenter());
			}
		}
		action = ACTION_WALK_RIGHT;

	}
	if (direction & UP) {
		if (grounded) {
			float impulse = body->GetMass();
			body->ApplyLinearImpulse(b2Vec2(0, -impulse * 8),
					body->GetWorldCenter());
			Mix_PlayChannel(-1, Game::sounds["player jump"], 0);
		}
		//action = ACTION_JUMP_LEFT;
	}

	if (!(direction & (LEFT | RIGHT | UP))) {
		feetFixture->SetFriction(200.0);
		action=ACTION_STAY;
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
