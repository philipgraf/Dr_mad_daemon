/*
 * Slot.cpp
 *
 *  Created on: 11.02.2013
 *      Author: philip
 */

#include "Slot.h"
using namespace std;

vector<Slot*> Slot::slots;

void Slot::loadSlots() {
	YAML::Node slots;
	try {
		slots = YAML::LoadFile(CONFIGS"slots.yml");
	} catch (YAML::Exception &e) {
		cout << e.msg << ": " << e.what() << endl;
	}
	int i = 0;

	for (YAML::iterator it = slots.begin(); it != slots.end(); ++it) {

		Slot slot;
		slot.name = it->first.Scalar();
		slot.finishedLevels = it->second["finished levels"].as<int>();
		slot.playerItems = it->second["items"].as<map<string, int> >();
		Slot::slots.push_back(new Slot(slot));
	}
	if (Game::curGame->settings.activeSlot < 0 || Game::curGame->settings.activeSlot >= Slot::slots.size()) {
		Game::curGame->settings.activeSlot = Slot::slots.size() - 1;
	}

}

void Slot::saveSlots() {

	YAML::Emitter out;

	out << YAML::BeginMap;

	for (int i = 0; i < slots.size(); i++) {
		out << YAML::Key << slots[i]->getName();
		out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "finished levels";
		out << YAML::Value << slots[i]->finishedLevels;
		out << YAML::Key << "items";
		out << YAML::Value << slots[i]->playerItems;
		out << YAML::EndMap;
	}

	out << YAML::EndMap;

	fstream filestream;
	filestream.open(CONFIGS"slots.yml", fstream::out);
	filestream << out.c_str();

	filestream.close();

}

Slot::Slot(string name) {
	this->name = name;
	finishedLevels = 0;
	//Slot::slotnames.push_back(this->name);
}

/*******************************************GETTER AND SETTER *************************************************/

void Slot::checkAndSetFinishedLevels(int levelnum) {
	if (levelnum == finishedLevels)
		finishedLevels++;
}

int Slot::getFinishedLevels() const {
	return finishedLevels;
}

void Slot::setFinishedLevels(int finishedLevels) {
	this->finishedLevels = finishedLevels;
}

const string& Slot::getName() const {
	return name;
}

map<string, int> Slot::getPlayerItems() {
	return playerItems;
}

void Slot::setPlayerItems(const std::map<std::string, int>& playerItems) {
	this->playerItems = playerItems;
}

void Slot::setName(const string& name) {
	this->name = name;
}

Slot::~Slot() {
	// TODO Auto-generated destructor stub
}

