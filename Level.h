
#include "includes.h"

#ifndef _LEVEL_H_
#define _LEVEL_H_


#define	BACKGROUND 0
#define	MAIN 1
#define	FOREGROUND 2


using namespace std;

class Level {
private:

	/**
	 * 3D Tilearray [0=bg,1=main,2=fg][x][y]
	 */
	Tile ****tilelist;
	string name;
	SDL_Surface *background;
	Entity* player; 
	int width;
	int height;
	int gravity;
	int time;
	Camera* mainCam;


	void renderLayer(int layer);
	
public:
	Level(string lname="l000");
	~Level();
	int getGravity() const;
	void setGravity(int gravity);
	const string& getName() const;
	int getTime() const;
	void setTime(int time);
	int getTileID(int x,int y,int layer=1);
	void render();
	void logic();


	Entity*& getPlayer() {
		return player;
	}

	int getHeight() const
	{
		return height;
	}

	int getWidth() const
	{
		return width;
	}
};


#endif
