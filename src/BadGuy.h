#ifndef BADGUY_H_
#define BADGUY_H_

#include <string>

#include "Entity.h"

using namespace std;

/** Badguy class.
 * This class defines all badguy specific things
 * @author Felix Eckner
 * @date 14.04.2013
 * @version 0.1.0 Alpha-State
 */
class BadGuy: public Entity {
public:
	BadGuy(string type, int x, int y);

	void move();
	void logic();

};

#endif /* BADGUY_H_ */
