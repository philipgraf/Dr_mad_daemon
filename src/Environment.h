/*
 * Environment.h
 *
 *  Created on: 28.02.2013
 *      Author: philip
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <string>

#include "Entity.h"


class Environment : public Entity {
public:
	Environment(std::string type, int x,int y);
};

#endif /* ENVIRONMENT_H_ */
