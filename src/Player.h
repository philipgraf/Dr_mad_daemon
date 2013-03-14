#ifndef PLAYER_H_
#define PLAYER_H_

#include "Entity.h"
#include "PDA.h"
#include "Language.h"
#include <Box2D/Box2D.h>
#include <map>
#include <string>
#include <cmath>

/** Player class.
 * This class handles the state of the player
 * @author Felix Eckner
 * @date 14.03.2013
 * @version 0.1.0 Alpha-State
 */
class Player: public Entity {
private:
	bool running;/**< is TRUE if the player is running*/
	bool jumping;/**< is true if the player is jumping*/
	unsigned selectedEntity; /**< the entityList index of the "active target" selected by the player*/
	int impactSoundPlayed; /**<  counter to prevent the impact sound from playing multiple times when the player impacts in the ground after jumping or falling*/
	b2RevoluteJoint *grebJoin; /**< connection between player and grabbed entity*/
	b2Vec2 *distanceVec; /**< Vector from player to grabbed objekt*/
public:
	Player(int x, int y, int level);
	virtual ~Player();

	PDA pda; /**< the players pda */

	Language lang; /**< this objekt is used for translation*/


	void use();
	void move();
	void grab();
	void logic();

	void addItem(std::string item);
	PDA &getpda();
	float getY() const;
	unsigned getSelectedEntity() const;
	bool isJumping() const;
	void setJumping(bool jumping);
	bool isRunning() const;
	void setRunning(bool running);
};

b2Vec2 addAngle(b2Vec2 vector, float angle,bool limit=true);
b2Vec2 setAngle(b2Vec2 vector, float angle);

#endif /* PLAYER_H_ */
