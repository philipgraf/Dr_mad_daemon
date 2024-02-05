#ifndef ENTITY_H_
#define ENTITY_H_

#include <vector>
#include <string>
#include <map>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <box2d/box2d.h>

//TODO find a better way
#define ACTION_STAY 0
#define ACTION_WALK_LEFT 1
#define ACTION_WALK_RIGHT 2
#define ACTION_JUMP_LEFT  3
#define ACTION_JUMP_RIGHT 4
#define ACTION_DUCK_LEFT 5
#define ACTION_DUCK_RIGHT 6
#define ACTION_DEAD 3

#define UP 0x01
#define RIGHT 0x02
#define DOWN 0x04
#define LEFT 0x08

/**
 * Entity Class.
 * This class handle all things you can interact with.
 * @author Felix Eckner
 * @date 14.04.2013
 * @version 0.1.0 Alpha-State
 */
class Entity {
private:
	Uint32 timer; /**< time in milliseconds needed for nextframe */
protected:
	std::map<std::string, int> items; /**< a map with all items and there amount */
	SDL_Surface * image; /**< Image of the entity with all animationframes */
	bool alive; /**< contains true if the entity is still alive and false if not */

	Uint8 direction;

	int currentframe; /**< Current Frame number this is used to calculate the correct part of the image */
	int action; /**< the action e.g. move left, move right, jump, needed for rendering  */
	std::vector<int> actionframes; /**< contains the number of frames for each action */
	std::vector<unsigned> animationDuration; /**< contains the duration of each actionframe */

	float width;/**< width of the entity in meter */
	float height;/**< height of the entity in meter */

	float maxVelocity; /**< the maximal velocity of the entity. May be manipulated for running, etc. **/

	bool grounded; /**< is true when sensorBottom collide with something. */

	b2Fixture *sensorRight; /**< right sensor used for collision detection  */
	b2Fixture *sensorLeft; /**< left sensor used for collision detection  */
	b2Fixture *sensorTop; /**< top sensor used for collision detection  */
	b2Fixture *sensorBottom; /**< bottom sensor used for collision detection  */

	b2Body *body; /**< the main body. contains the x and y values and to this body all forces (gravity, ...) are applied */

	std::vector<b2Fixture*> wheels; /** vector of all bottom shapes for a smoother movement */
	void nextframe();

public:
	static std::vector<Entity*> entityList; /**< list of all entities */

	Entity();
	virtual ~Entity();

	virtual void logic();
	virtual void move();

	int checkCollision();

	//----------------------------------- Getter and Setter ------------------------------

	SDL_Rect getCurFrameRect();
	bool isAlive() const;
	void setAlive(bool alive);
	float getX() const;
	float getY() const;
	SDL_Surface* getImage();
	int getCurrentframe() const;
	void setCurrentframe(int currentframe);
	float getHeight() const;
	void setHeight(float height);
	float getWidth() const;
	void setWidth(float width);
	b2Body* getBody();

	void setDirection(Uint8 direction);
	void delDirection(Uint8 direction);
	Uint8 getDirection() const;
	std::map<std::string, int>& getItems();
	virtual void addItem(std::string item);
};

#endif /* ENTITY_H_ */
