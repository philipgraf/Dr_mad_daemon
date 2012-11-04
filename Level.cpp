
#include "Level.h"

Level::Level(string lname) {
	fstream filestream;
	string background;
	filestream.open(("levels/"+lname+".conf").c_str(),fstream::in);
	filestream >> name >> width >> height >> background >> gravity >> time;

	background = "img/"+background;

	this->background = Tools::loadImage(background);

	filestream.close();

	//TODO array initialisieren

}

Level::~Level() {
}

int Level::getGravity() const {
	return gravity;
}

void Level::setGravity(int gravity) {
	this->gravity = gravity;
}

const string& Level::getName() const {
	return name;
}

int Level::getTime() const {
	return time;
}

void Level::setTime(int time) {
	this->time = time;
}

