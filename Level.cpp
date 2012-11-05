
#include "Level.h"

Level::Level(string lname) {
	width=height=gravity=time=0;
	tilelist=NULL;

	fstream filestream;
	string background;
	filestream.open(("levels/"+lname+".conf").c_str(),fstream::in);
	filestream >> name >> width >> height >> background >> gravity >> time;

	background = "img/"+background;

	this->background = Tools::loadImage(background);

	filestream.close();

	tilelist = new Tile***[3];
	for(int i=0;i<3;i++){
		tilelist[i] =new Tile**[width];
		for(int j=0;j<width;j++){
			tilelist[i][j]= new Tile*[height];
		}
	}


	filestream.open(("levels/"+lname+".map").c_str(),fstream::in);
	for(int i=0;i<3;i++){
		for(int y=0;y<height;y++){
			for(int x=0;x<width;x++){
				int id;
				filestream >> id;
				tilelist[i][x][y]= new Tile(id);
			}
		}
		char ch;
		filestream >> ch;
		if(ch!=';')
			Tools::error("Error loading mapfile:levels/"+lname+".map"); // TODO Throw Errorobjekt
	}
	filestream.close();

}

Level::~Level() {
	for(int i=0;i<3;i++){
		for(int y=0;y<height;y++){
			for(int x=0;x<width;x++){
				delete tilelist[i][y][x];
			}
			delete [] tilelist[i][y];
		}
		delete [] tilelist[i];
	}
	delete [] tilelist;
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

int Level::getTileID(int x, int y, int layer) {
	return tilelist[layer][x][y]->getId();
}

void Level::render(SDL_Rect view,int layer) {
	Tools::drawImage(SDL_GetVideoSurface(), 0, 0, background, view.x, view.y, view.w,
				view.h);
	for(int y=0;y<height;y++){
		for(int x=0;x<width;x++){
			tilelist[layer][x][y]->render(x,y);
		}
	}

}
