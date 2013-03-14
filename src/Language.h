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

class Language {
public:
	static std::vector<std::string> supLanguages; /**< a list with all supported Languages */
private:
	YAML::Node root; /**< the root node of the lang.yml file */

public:
	Language();
	std::string operator[](std::string key);
	static void getSupportedLanguages();

};

#endif /* LANGUAGE_H_ */
