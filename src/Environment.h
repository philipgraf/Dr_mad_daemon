#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <string>

#include "Entity.h"

/**
 * Environment Class.
 * This class defines all Environment specific things
 * @author Felix Eckner
 * @date 14.04.2013
 * @version 0.1.0 Alpha-State
 */
class Environment : public Entity {
public:
	Environment(std::string type, int x,int y);
};

#endif /* ENVIRONMENT_H_ */
