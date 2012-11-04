
#include "Level.h"

Level::Level(string lname) {
	printf("Test");
	fstream filestream;
	//filestream.open(("levels/"+lname+".conf").c_str(),fstream::in);
	//filestream >> name >> width;

	cout << name << endl;
	cout << width << endl;

	/**
	 * 	name=tutorial
	 *	width=50
	 *	height=50
	 *	background=bg.png
	 * 	gravity=1
	 *	time=300
	 */



	//filestream.close();

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

