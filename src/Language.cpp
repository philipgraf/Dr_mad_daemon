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
	return root[key][language].Scalar();
}

void Language::setLanguage(std::string language) {
	this->language = language;
}
