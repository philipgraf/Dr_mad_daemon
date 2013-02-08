/*
 * Language.cpp
 *
 *  Created on: 07.02.2013
 *      Author: philip
 */

#include "Language.h"
#include "define.h"
#include "Game.h"

Language::Language() {

	root = LoadFile(CONFIGS"lang.yml");
	language = Game::curGame->settings.language;

}

Language::~Language() {

}

std::string Language::operator [](std::string key) {

	std::string retVel;
	if(root[key][language].Scalar() == detail::node_data::empty_scalar){
		return root[key]["en"].Scalar();
	}else{
		return root[key][language].Scalar();

	}
}

void Language::setLanguage(std::string language) {
	this->language = language;
}
