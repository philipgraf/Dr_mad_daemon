#include "Player.h"
#include "Tile.h"
#include "Game.h"
#include "Slot.h"
#include "BadGuy.h"
#include "Notification.h"

#include <sstream>

using namespace std;

/**
 * Constructor of Player.
 * Sets size, animation parameters, some physical attributes and load image
 * Also the "physical body is generated and sensors for collision detection are added
 *
 * @param x horizontal position
 * @param y vertical position
 * @param level level of the pda
 */
Player::Player(int x, int y, int level) :
		Entity(), pda(level) {
	width = 1;
	height = 2;

	grebJoin = NULL;
	distanceVec = NULL;

	actionframes.push_back(1);
	actionframes.push_back(8);
	actionframes.push_back(8);

	animationDuration.push_back(9999);
	animationDuration.push_back(60);
	animationDuration.push_back(60);

	float halfWidth = width / 2;
	float halfHeight = height / 2;

	maxVelocity = 5;

	selectedEntity = 0;

	running = false;
	jumping = false;

	impactSoundPlayed = 0;

	items = Slot::slots[Game::curGame->settings.activeSlot]->getPlayerItems();

	SDL_Surface *tmp = SDL_LoadBMP(IMG"player.bmp");

	if (!tmp) {
		//TODO Throw Exception
		cout << "unable to load BMP file player.bmp" << endl;
	} else {
		image = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		if (image != 0) {
			SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(image->format, 255, 0, 255));

		}
	}

	direction = 0;

	float radius = halfWidth;

	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.fixedRotation = true;
	bodydef.position.Set(x + halfWidth, y + halfHeight);
	this->body = Game::curGame->getCurrentLevel()->getWorld()->CreateBody(&bodydef);

	body->GetUserData().pointer = (uintptr_t) this;

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(halfWidth, halfHeight - radius / 2);

	b2FixtureDef *fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &dynamicBox;
	fixtureDef->density = 40.0f;
	fixtureDef->friction = 0.0f;
	fixtureDef->filter.categoryBits = 2;
	body->CreateFixture(fixtureDef);

	b2CircleShape feetShape;
	feetShape.m_radius = radius;
	feetShape.m_p = b2Vec2(0, halfWidth + radius / 2);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &feetShape;
	fixtureDef->density = 40.0f;
	fixtureDef->friction = 0.2;
	fixtureDef->filter.categoryBits = 2;

	wheels.push_back(body->CreateFixture(fixtureDef));

	b2PolygonShape sensorRight;
	sensorRight.SetAsBox(0.01, halfHeight - radius / 2, b2Vec2(halfWidth, 0.2), 0);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &sensorRight;
	fixtureDef->isSensor = true;
	fixtureDef->filter.categoryBits = 2;
	this->sensorRight = body->CreateFixture(fixtureDef);

	b2PolygonShape sensorLeft;
	sensorLeft.SetAsBox(0.01, halfHeight - radius / 2, b2Vec2(-halfWidth, 0.2), 0);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &sensorLeft;
	fixtureDef->isSensor = true;
	fixtureDef->filter.categoryBits = 2;
	this->sensorLeft = body->CreateFixture(fixtureDef);

	b2PolygonShape sensorTop;
	sensorTop.SetAsBox(halfWidth - 0.1, 0.01, b2Vec2(0, -halfHeight + radius / 2), 0);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &sensorTop;
	fixtureDef->isSensor = true;
	fixtureDef->filter.categoryBits = 2;
	this->sensorTop = body->CreateFixture(fixtureDef);

	b2CircleShape sensorBottom;
	sensorBottom.m_radius = radius - 0.05;
	sensorBottom.m_p = b2Vec2(0, halfWidth + radius / 2 + 0.05);

	fixtureDef = new b2FixtureDef;
	fixtureDef->shape = &sensorBottom;
	fixtureDef->isSensor = true;
	fixtureDef->filter.categoryBits = 2;
	this->sensorBottom = body->CreateFixture(fixtureDef);

}

/**
 * Handles the "use"-logic.
 * if a dead object is below the player it will be looted (items will be thrown out and entity will be deleted).
 * if the player is in front of a tile with a flag switchactions will be executed
 */
void Player::use() {
	Entity *deadguy;
	// check player contacts with dead bodys and loot them
	for (b2ContactEdge *contactEdge = body->GetContactList(); contactEdge; contactEdge = contactEdge->next) {
		if (contactEdge->contact->GetFixtureA() == sensorBottom && contactEdge->contact->IsTouching()) {
			if ((deadguy = (Entity*) contactEdge->contact->GetFixtureB()->GetBody()->GetUserData().pointer) != NULL) {
				delete deadguy;
				break;
			}
		} else if (contactEdge->contact->GetFixtureB() == sensorBottom && contactEdge->contact->IsTouching()) {
			if ((deadguy = (Entity*) contactEdge->contact->GetFixtureA()->GetBody()->GetUserData().pointer) != NULL) {
				delete deadguy;
				break;
			}
		}
	}

	unsigned flags = Game::curGame->getCurrentLevel()->getTilelist()[0][(int) body->GetPosition().x][(int) body->GetPosition().y]->getFlags();

	// check player position an get the flags of the tiles there
	if (flags & 0xFF) {
		Game::curGame->getCurrentLevel()->toggleSwitch(flags);
		Game::curGame->getCurrentLevel()->switchActions();
	}

	// if flag is finishpoint
	if (flags & TF_FINISH) {
		if (Game::curGame->getCurrentLevel()->getSwitches() == 0xFF) {
			Game::curGame->getCurrentLevel()->setFinished(true);
		}
	}
}
/**
 * Handles the player movement
 *
 * checkes the wanted direction of movement and applies the needed forces to the player
 * also handles the the aiming if a dead body is grabbed
 */
void Player::move() {
	if (running) {
		maxVelocity = 8;
	} else {
		maxVelocity = 5;
	}

	if (direction & LEFT) {
		wheels[0]->SetFriction(0.2);
		if (checkCollision() & DOWN) {
			if (body->GetLinearVelocity().x > -maxVelocity) {
				body->ApplyLinearImpulse(b2Vec2(-body->GetMass() / 2, 0), body->GetWorldCenter(),true);
			}
		} else {
			if (body->GetLinearVelocity().x > -maxVelocity) {
				body->ApplyLinearImpulse(b2Vec2(-body->GetMass() / 8, 0), body->GetWorldCenter(),true);
			}
		}

		action = ACTION_WALK_LEFT;

	}
	if (direction & RIGHT) {
		wheels[0]->SetFriction(0.2);
		if (checkCollision() & DOWN) {
			if (body->GetLinearVelocity().x < maxVelocity) {
				body->ApplyLinearImpulse(b2Vec2(body->GetMass() / 2, 0), body->GetWorldCenter(),true);
			}
		} else {
			if (body->GetLinearVelocity().x < maxVelocity) {
				body->ApplyLinearImpulse(b2Vec2(body->GetMass() / 8, 0), body->GetWorldCenter(),true);
			}
		}
		action = ACTION_WALK_RIGHT;

	}

	if ((direction & UP) && grebJoin != NULL) {
		if (selectedEntity != 0) {
			Game::curGame->getCurrentLevel()->getWorld()->DestroyJoint(grebJoin);
			*distanceVec = addAngle(*distanceVec, 5);
			entityList[selectedEntity]->getBody()->SetTransform(body->GetPosition() + *distanceVec, 0.0);
			b2RevoluteJointDef rjd;
			rjd.Initialize(entityList[selectedEntity]->getBody(), body, body->GetWorldCenter());

			grebJoin = (b2RevoluteJoint*) Game::curGame->getCurrentLevel()->getWorld()->CreateJoint(&rjd);
		}
	}

	if ((direction & DOWN) && grebJoin != NULL) {
		if (selectedEntity != 0) {
			Game::curGame->getCurrentLevel()->getWorld()->DestroyJoint(grebJoin);

			*distanceVec = addAngle(*distanceVec, -5);
			entityList[selectedEntity]->getBody()->SetTransform(body->GetPosition() + *distanceVec, 0.0);
			b2RevoluteJointDef rjd;
			rjd.Initialize(entityList[selectedEntity]->getBody(), body, body->GetWorldCenter());

			grebJoin = (b2RevoluteJoint*) Game::curGame->getCurrentLevel()->getWorld()->CreateJoint(&rjd);
		}
	}

	if (jumping) {
		if (checkCollision() & DOWN) {
			float impulse = body->GetMass();
			body->ApplyLinearImpulse(b2Vec2(0, -impulse * 4), body->GetWorldCenter(),true);
			Mix_PlayChannel(-1, Game::sounds["player jump"], 0);
		}
		//action = ACTION_JUMP_LEFT;
	}

	if ((!(direction & (LEFT | RIGHT)) && !jumping)) {
		wheels[0]->SetFriction(10.0);
		action = ACTION_STAY;
	}

}

/**
 * Handles joins between player and dead entities
 * if the selected target isn't already attached to the player this method will generate a join (pic up) to the targeted entity.
 * on the other hand if there is already a join, it will be deleted and a force will be applied (throw away)
 *
 */
void Player::grab() {
	if (selectedEntity != 0) {
		b2Body *selectedBody = entityList[selectedEntity]->getBody();
		if (grebJoin == NULL) {
			distanceVec = new b2Vec2(entityList[selectedEntity]->getBody()->GetWorldCenter() - body->GetWorldCenter());

			*distanceVec = addAngle(*distanceVec, 45, false);

			selectedBody->SetTransform(body->GetPosition() + *distanceVec, 0.0);
			b2RevoluteJointDef rjd;
			rjd.Initialize(entityList[selectedEntity]->getBody(), body, body->GetWorldCenter());
			//rjd.enableLimit = true;
			rjd.lowerAngle = -M_PI;
			rjd.upperAngle = M_PI;

			grebJoin = (b2RevoluteJoint*) Game::curGame->getCurrentLevel()->getWorld()->CreateJoint(&rjd);

		} else {
			b2Vec2 force = *distanceVec;
			force.x *= 10000;
			force.y *= 10000;
			Game::curGame->getCurrentLevel()->getWorld()->DestroyJoint(grebJoin);
			grebJoin = NULL;
			delete distanceVec;
			distanceVec = NULL;
			selectedBody->ApplyForceToCenter(force,false);
		}
	}
}

/**
 * Handles the logic related to the player.
 * This includes targeting to check if there is a grabable dead entity(boxes or dead badguy) in range  and select it,
 * call checkCollision() method to check the collisions,
 * kill the player if there is a collision with a badguy left or right or it is out of the level,
 * check if player is grounded
 * and call the Entity::logic
 *
 * @see Entity::checkCollision()
 * @see Entity::logic()
 */
void Player::logic() {
//TODO check connected tiles for shock and other game events
	if (pda.getLevel() > 0 /*& items["gravitiy module"] > 0*/) {
		if (grebJoin == NULL) {
			if (selectedEntity == 0) {
				for (unsigned i = 0; i < entityList.size(); i++) {
					// get all dead entities within 3 m
					//TODO get the range from PDA
					if (!entityList[i]->isAlive() && (entityList[i]->getBody()->GetWorldCenter() - this->getBody()->GetWorldCenter()).Length() < 3.0) {
						selectedEntity = i;
						break;
					}
				}
				// check if selected entity is out of range
			} else if (selectedEntity >= entityList.size() || ((selectedEntity > 0) && (entityList[selectedEntity]->getBody()->GetWorldCenter() - this->getBody()->GetWorldCenter()).Length() > 3.0)) {
				selectedEntity = 0;
			}
		}
	}

	//int collision = checkCollision();

	checkCollision();

	for (b2ContactEdge *contactEdge = body->GetContactList(); contactEdge; contactEdge = contactEdge->next) {
		if ((contactEdge->contact->GetFixtureA() == sensorRight || contactEdge->contact->GetFixtureA() == sensorLeft || contactEdge->contact->GetFixtureA() == sensorTop) && contactEdge->contact->IsTouching()) {
			if (contactEdge->contact->GetFixtureB()->GetBody()->GetUserData().pointer != NULL) {
				if (((Entity*) contactEdge->contact->GetFixtureB()->GetBody()->GetUserData().pointer)->isAlive()) {
					alive = false;
				}
			}
		} else if ((contactEdge->contact->GetFixtureB() == sensorRight || contactEdge->contact->GetFixtureB() == sensorLeft || contactEdge->contact->GetFixtureB() == sensorTop) && contactEdge->contact->IsTouching()) {
			if (contactEdge->contact->GetFixtureA()->GetBody()->GetUserData().pointer != NULL) {
				if (((Entity*) contactEdge->contact->GetFixtureA()->GetBody()->GetUserData().pointer)->isAlive()) {
					alive = false;
				}
			}
		}
	}

	if (body->GetPosition().y - height / 2 > Game::curGame->getCurrentLevel()->getHeight()) {
		alive = false;
	}

	if (grounded && impactSoundPlayed > 20) {
		Mix_PlayChannel(-1, Game::sounds["player jump impact"], 0);
	} else if (!grounded) {
		impactSoundPlayed++;
	}
	if (grounded) {
		impactSoundPlayed = 0;
	}

	Entity::logic();
}

/**
 * Handles the itemcollection
 * This will call the Entity::addItem() and throw a Notification
 *
 * @see Entity::addItem()
 * @see Notification()
 */
void Player::addItem(string item) {
	Entity::addItem(item);

	std::stringstream s;
	s << lang["looted"] << " " << lang[item];
	new Notification(s.str(), 2, NOTIFICATION_INFO, item);
}
/**
 * Returns the Players PDA
 *
 * @see pda
 */
PDA &Player::getpda() {
	return pda;
}

/**
 * Returns TRUE if Player is currently jumping
 *
 * @see jumping
 */
bool Player::isJumping() const {
	return jumping;
}

/**
 * Sets Player to jumping
 *
 * @param jumping
 *
 * @see jumping
 */
void Player::setJumping(bool jumping) {
	this->jumping = jumping;
}

/**
 * Returns TRUE if Player is running
 *
 * @see running
 */
bool Player::isRunning() const {
	return running;
}

/**
 * Sets Player to running
 *
 * @param jumping
 *
 * @see jumping
 */
void Player::setRunning(bool running) {
		this->running = running;
}

/**
 * Returns the entityList index of the currently selected(targeted) Entity
 *
 * @see selectedEntity
 */
unsigned Player::getSelectedEntity() const {
	return selectedEntity;
}

/**
 * Returns the current vertical position of the Player
 */
float Player::getY() const {
	return body->GetPosition().y + height / 8;
}
/**
 * Rotates a Vector
 */
b2Vec2 addAngle(b2Vec2 vector, float angle, bool limit) {
	b2Vec2 newVector;

	float r = vector.Length();
	float phi = atan2f(vector.y, vector.x);
	if ((!limit) || (phi <= 0.0 && phi > -M_PI)) {
		if (vector.x > 0)
			phi -= (angle * (M_PI / 180));
		else {
			phi += (angle * (M_PI / 180));
		}
	} else if (phi > 0 && phi < M_PI_2) {
		phi = 0;
	} else if (phi <= -M_PI || phi > M_PI_2) {
		phi = -M_PI + 0.01;
	}

	newVector.x = r * cosf(phi);
	newVector.y = r * sinf(phi);

	return newVector;
}


//b2Vec2 setAngle(b2Vec2 vector, float angle) {
//	b2Vec2 newVector;
//
//	float r = vector.Length();
//	float phi = angle * (M_PI / 180);
//
//	newVector.x = r * cosf(phi);
//	newVector.y = r * sinf(phi);
//
//	return newVector;
//}
