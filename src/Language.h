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
#include <vector>

using namespace YAML;

class Language {
public:
	static std::vector<std::string> supLanguages;
private:
	Node root;

public:
	Language();
	virtual ~Language();
	std::string operator[](std::string key);
	static void getSupportedLanguages();

};

#endif /* LANGUAGE_H_ */
