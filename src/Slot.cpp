#include "Slot.h"
#include <cstdlib>
using namespace std;

vector<Slot*> Slot::slots;

/**
 * Constructor of Slot.
 *
 * @param name the name of the saveslot (DrInSane is default)
 */
Slot::Slot(string name) {
	this->name = name;
	finishedLevels = 0;
	pdaLevel=0;
}

/**
 * Load existing slots.
 * all the already existing slots in slot.yml will be loaded and stored in slots
 *
 * @see slots
 */
void Slot::loadSlots() {

	string home="";
	if(getenv("HOME")!=NULL){
		home= getenv("HOME");
	}else{
		home=".";
	}

	YAML::Node slots;
	try {
		slots = YAML::LoadFile(home+"/.DIS/slots.yml");
	} catch (YAML::Exception &e) {
		cout << e.msg << ": " << e.what() << endl;
	}
	for (YAML::iterator it = slots.begin(); it != slots.end(); ++it) {

		Slot slot;
		slot.name = it->first.Scalar();
		slot.finishedLevels = it->second["finished levels"].as<int>();
		slot.playerItems = it->second["items"].as<map<string, int> >();
		slot.pdaLevel = it->second["pda level"].as<int>();
		Slot::slots.push_back(new Slot(slot));
	}
	if (Game::curGame->settings.activeSlot < 0 || Game::curGame->settings.activeSlot >= (int) Slot::slots.size()) {
		Game::curGame->settings.activeSlot = Slot::slots.size() - 1;
	}

}
/**
 * Save all Slots to YAML-File.
 * All the Slots in slots will be converted to YAML format and stored in slots.yml
 *
 * @see slots
 */
void Slot::saveSlots() {

	YAML::Emitter out;

	out << YAML::BeginMap;

	for (unsigned i = 0; i < slots.size(); i++) {
		out << YAML::Key << slots[i]->getName();
		out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "finished levels";
		out << YAML::Value << slots[i]->finishedLevels;
		out << YAML::Key << "items";
		out << YAML::Value << slots[i]->playerItems;
		out << YAML::Key << "pda level";
		out << YAML::Value << slots[i]->pdaLevel;
		out << YAML::EndMap;

	}
	out << YAML::EndMap;

	string home="";
		if(getenv("HOME")!=NULL){
			home= getenv("HOME");
		}else{
			home=".";
		}


	fstream filestream;
	filestream.open((home+"/.DIS/slots.yml").c_str(), fstream::out);
	filestream << out.c_str();

	filestream.close();

}


/*******************************************GETTER AND SETTER *************************************************/
/**
 * Unlocks the next level
 * If a Level is finished successful and the level is the last playable in the current slot, the next level will be unlocked.
 *
 * @param levelnum the number of the finished level
 *
 * @see finishedLevels
 */
void Slot::checkAndSetFinishedLevels(int levelnum) {
	if (levelnum == finishedLevels)
		finishedLevels++;
}

/**
 * Returns the number of the highest finished level in this slot
 *
 * @see finishedLevels
 */
int Slot::getFinishedLevels() const {
	return finishedLevels;
}

/**
 * Returns the name of the slot.
 *
 * @see name
 */
const string& Slot::getName() const {
	return name;
}

/**
 * Returns the PDA-Level of the slot.
 *
 * @see pdaLevel
 */
int Slot::getPdaLevel() const {
	return pdaLevel;
}

/**
 * Sets the level of the PDA.
 *
 * @param pdaLevel the new PDA-Level
 *
 * @see pdaLevel
 */
void Slot::setPdaLevel(int pdaLevel) {
	this->pdaLevel = pdaLevel;
}

/**
 * Returns the items of the player.
 * A map of all Player-items stored in the slot will be returned
 *
 * @see playerItems
 */
map<string, int>& Slot::getPlayerItems() {
	return playerItems;
}

/**
 * Sets a new map with playeritems.
 *
 * @param playerItems reference to a map with playerItems
 *
 * @see playerItems
 */
void Slot::setPlayerItems(const std::map<std::string, int>& playerItems) {
	this->playerItems = playerItems;
}

/**
 * Sets the name of a Slot
 *
 * @param name reference to a string
 *
 * @see name
 */
void Slot::setName(const string& name) {
	this->name = name;
}
