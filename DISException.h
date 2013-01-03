/*
 * DISException.h
 *
 *  Created on: 16.12.2012
 *      Author: philip
 */

#ifndef DISEXCEPTION_H_
#define DISEXCEPTION_H_

#include "includes.h"

class DISException: public std::exception {
public:
	DISException();
	virtual ~DISException();
};

#endif /* DISEXCEPTION_H_ */
