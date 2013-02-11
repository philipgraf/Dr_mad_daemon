/*
 * Slot.cpp
 *
 *  Created on: 11.02.2013
 *      Author: philip
 */

#include "Slot.h"

vector<Slot*> Slot::slots;

void Slot::loadSlots() {
	YAML::Node slots;
	try {
		slots = YAML::LoadFile(CONFIGS"slots.yml");
	} catch (...) {
		//TODO create new default slots.yml
	}
	int i=0;

	for (YAML::iterator it = slots.begin(); it != slots.end(); ++it) {

		Slot slot;
		slot.setName(it->first.Scalar());
		slot.setFinishedLevels(it->second["finished levels"].as<int>());
		Slot::slots.push_back(new Slot(slot));
		if(Game::curGame->settings.activeSlot == -1){
			Game::curGame->settings.activeSlot = i++;
		}
	}

}

void Slot::saveSlots(){

	YAML::Emitter out;

	out << YAML::BeginMap;

	for(int i=0; i< slots.size() ; i++){
		out << YAML::Key << slots[i]->getName();
		out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "finished levels";
		out << YAML::Value << slots[i]->finishedLevels;
		out << YAML::EndMap;
	}

	out << YAML::EndMap;

	fstream filestream;
	filestream.open(CONFIGS"slots.yml");
	filestream << out.c_str();

	filestream.close();

}

Slot::Slot(string name) {
	this->name = name;
	finishedLevels = 0;
	//Slot::slotnames.push_back(this->name);
}

Slot::Slot(Slot &copy){
	this->name=copy.name;
	this->finishedLevels = copy.finishedLevels;
}

/*******************************************GETTER AND SETTER *************************************************/

void Slot::checkAndSetFinishedLevels(int levelnum) {
	if(levelnum == finishedLevels)
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

void Slot::setName(const string& name) {
	this->name = name;
}

Slot::~Slot() {
	// TODO Auto-generated destructor stub
}

