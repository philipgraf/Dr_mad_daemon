/*
 * Language.cpp
 *
 *  Created on: 07.02.2013
 *      Author: philip
 */

#include "Language.h"
#include "define.h"
#include "Game.h"

vector<string> Language::supLanguages;

using namespace YAML;

/**
 * Load the language configuration file
 */
Language::Language() {

	root = LoadFile(CONFIGS"lang.yml");

}

/**
 * get the translation of the given string. try to return the language which is stores in the game settings if this translation is not available the engish translation will be returned.
 * @param key the key in the root note
 * @see root
 * @return the translated string
 */
std::string Language::operator [](std::string key) {

	std::string retVel;
	if (root[key][Game::curGame->settings.language].Scalar() == detail::node_data::empty_scalar) {
		return root[key]["en"].Scalar();
	} else {
		return root[key][Game::curGame->settings.language].Scalar();

	}
}

/**
 * get all languages from the greeting entry and store it in supLanguages
 * @see supLanguages
 */
void Language::getSupportedLanguages() {
	Node root = LoadFile(CONFIGS"lang.yml");

	for (YAML::iterator it = root["greeting"].begin(); it != root["greeting"].end(); ++it) {
		supLanguages.push_back(it->first.Scalar());
	}

}
