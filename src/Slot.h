/*
 * Slot.h
 *
 *  Created on: 11.02.2013
 *      Author: philip
 */

#ifndef SLOT_H_
#define SLOT_H_

#include <string>
#include <yaml-cpp/yaml.h>

#include "define.h"
#include "Game.h"

using namespace std;

class Slot {
public:
	static vector<Slot*> slots;
	static int currentSlot;

	static void loadSlots();
	static void saveSlots();
private:
	string name;
	int finishedLevels;

public:
	Slot(string name="DrInSane");
	Slot(Slot &copy);
	virtual ~Slot();


	void checkAndSetFinishedLevels(int levelnum);
	int getFinishedLevels() const;
	void setFinishedLevels(int finishedLevels);
	const string& getName() const;
	void setName(const string& name);
};

#endif /* SLOT_H_ */
