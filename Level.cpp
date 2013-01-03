
#include "Level.h"

Level::Level(string lname) {

	width=height=gravity=time=0;

	tilelist=NULL;
	background=NULL;

	//TODO: get width and height dynamically
	player = new Entity("img/player.png",32,64);
	string background;

	fstream filestream;
	filestream.open(("levels/"+lname+".conf").c_str(),fstream::in);
	filestream >> name >> width >> height >> background >> gravity >> time;
	filestream.close();

	background = "img/"+background;

	this->background = Tools::loadImage(background);


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
				u_int64_t id;
				filestream >> id;

				if(id & TF_START){
					player->setX(x*TILESIZE);
					player->setY(y*TILESIZE);
				}

				tilelist[i][x][y]= new Tile(id);

			}
		}
		char ch;
		filestream >> ch;
		if(ch!=';')
			Tools::error("Error loading mapfile:levels/"+lname+".map"); // TODO Throw Errorobjekt
	}
	filestream.close();

	mainCam = new Camera(player);

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

void Level::renderLayer(int layer) {
	for(int y=0;y<height;y++){
		for(int x=0;x<width;x++){
			tilelist[layer][x][y]->render(x,y,mainCam->getRect());
		}
	}
}

void Level::render() {
	Tools::drawImage(SDL_GetVideoSurface(), 0, 0, background, mainCam->getX()/2, mainCam->getY()/2, mainCam->getWidth(),
				mainCam->getHeight());
	renderLayer(BACKGROUND);
	renderLayer(MAIN);
	mainCam->drawImage();
	//player->render();
	renderLayer(FOREGROUND);

}

void Level::logic() {
	int px,py;
	px = player->getX();
	py = player->getY();

//	if ((py+64>testrec.y && py<testrec.y+testrec.h && px +32 > testrec.x && px <testrec.x + testrec.w)){
//		if(player->getSpeedY()<0){
//			player->setY(testrec.y+testrec.h);
//			player->setSpeedY(0);
//			cout << "bottom"<< endl;
//		}else if(player->getSpeedY()>0){
//			player->setY(testrec.y-player->getImage()->clip_rect.h);
//			player->setSpeedY(0);
//			cout << "top"<< endl;
//		}
//		if(player->getSpeedX()>0){
//			player->setSpeedX(0);
//			player->setX(testrec.x-player->getImage()->clip_rect.w);
//			cout << "left"<< endl;
//		}
//		if(player->getSpeedX()<0){
//			player->setX(testrec.x+testrec.w);
//			player->setSpeedX(0);
//			cout << "right"<< endl;
//		}
//	}

	player->move();
	mainCam->logic();


}
