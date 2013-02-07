/*
 * Language.cpp
 *
 *  Created on: 07.02.2013
 *      Author: philip
 */

#include "Language.h"
#include "define.h"

Language::Language() {

	root = LoadFile(CONFIGS"lang.yml");
	lang = "de";

}

Language::~Language() {

}

std::string Language::operator [](std::string key) {
	return root[key][lang].Scalar();
}
