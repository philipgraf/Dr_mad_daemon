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

Language::Language() {

	root = LoadFile(CONFIGS"lang.yml");

}

Language::~Language() {

}

std::string Language::operator [](std::string key) {

	std::string retVel;
	if(root[key][Game::curGame->settings.language].Scalar() == detail::node_data::empty_scalar){
		return root[key]["en"].Scalar();
	}else{
		return root[key][Game::curGame->settings.language].Scalar();

	}
}

void Language::getSupportedLanguages() {
	YAML::Node root = YAML::LoadFile(CONFIGS"lang.yml");

	for (YAML::iterator it = root["greeting"].begin();
			it != root["greeting"].end(); ++it) {
		supLanguages.push_back(it->first.Scalar());
	}

}
