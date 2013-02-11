/*
 * Slot.cpp
 *
 *  Created on: 11.02.2013
 *      Author: philip
 */

#include "Slot.h"

vector<string> Slot::slotnames;
map<string, Slot*> Slot::slots;

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
		slot.setFinishedLevels(it->second["finished level"].as<int>());
		Slot::slots[it->first.Scalar()] = new Slot(slot);
		if(Game::curGame->settings.activeSlot == -1){
			Game::curGame->settings.activeSlot = i++;
		}
	}

	cout << Slot::slotnames.size() << endl;

}

void Slot::saveSlots(){

	for(map<string,Slot*>::iterator it=Slot::slots.begin(); it != Slot::slots.end(); ++it){

	}

}

Slot::Slot(string name) {
	this->name = name;
	finishedLevels = 0;
	//Slot::slotnames.push_back(this->name);
}

Slot::Slot(Slot &copy){
	this->name=copy.name;
	this->finishedLevels = copy.finishedLevels;
	Slot::slotnames.push_back(name);
}

/*******************************************GETTER AND SETTER *************************************************/

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

