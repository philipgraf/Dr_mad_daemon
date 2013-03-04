/*
 * Slot.h
 *
 *  Created on: 11.02.2013
 *      Author: philip
 */

#ifndef SLOT_H_
#define SLOT_H_

#include <string>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <map>

#include "define.h"
#include "Game.h"

class Slot {
public:
	static std::vector<Slot*> slots;

	static void loadSlots();
	static void saveSlots();
private:
	std::string name;
	int finishedLevels;
	std::map<std::string, int> playerItems;
	int pdaLevel;
public:
	Slot(std::string name = "DrInSane");
	virtual ~Slot();

	void checkAndSetFinishedLevels(int levelnum);
	int getFinishedLevels() const;
	void setFinishedLevels(int finishedLevels);
	const std::string& getName() const;
	void setName(const std::string& name);
	void setPlayerItems(const std::map<std::string, int>& playerItems);
	std::map<std::string, int> getPlayerItems();
	int getPdaLevel() const;
	void setPdaLevel(int pdaLevel);
};

#endif /* SLOT_H_ */
