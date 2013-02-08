/*
 * Language.h
 *
 *  Created on: 07.02.2013
 *      Author: philip
 */

#ifndef LANGUAGE_H_
#define LANGUAGE_H_

#include <yaml-cpp/yaml.h>
#include <string>

using namespace YAML;

class Language {
private:
	Node root;

public:
	Language();
	virtual ~Language();
	std::string operator[](std::string key);

};

#endif /* LANGUAGE_H_ */
