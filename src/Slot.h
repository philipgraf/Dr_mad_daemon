#ifndef SLOT_H_
#define SLOT_H_

#include <string>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <map>

#include "define.h"
#include "Game.h"

/** Slot class.
 * This class handles the "Savegame"-Slots and stores the game-progress
 * @author Felix Eckner
 * @date 14.03.2013
 * @version 0.1.0 Alpha-State
 */
class Slot {
public:
	static std::vector<Slot*> slots;/**< Stores all Slots*/

	static void loadSlots();
	static void saveSlots();
private:
	std::string name;/**< name of the slot(player)*/
	int finishedLevels;/**< the number of the highest successfully finished level*/
	std::map<std::string, int> playerItems;/**< all the items the player already collected*/
	int pdaLevel;/**< the upgadelevel of the pda*/
public:
	Slot(std::string name = "DrInSane");

	void checkAndSetFinishedLevels(int levelnum);
	int getFinishedLevels() const;
	void setFinishedLevels(int finishedLevels);
	const std::string& getName() const;
	void setName(const std::string& name);
	void setPlayerItems(const std::map<std::string, int>& playerItems);
	std::map<std::string, int>& getPlayerItems();
	int getPdaLevel() const;
	void setPdaLevel(int pdaLevel);
};

#endif
