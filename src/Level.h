
#include "includes.h"

#ifndef _LEVEL_H_
#define _LEVEL_H_





using namespace std;

/** Level class.
 * This class contain all Tiles, the name, the Background Image, the Player, absolute width and height in Tiles, the gravity and the time.
 * All values loaded from file.
 * @author Philip Graf
 * @date 04.11.2012
 * @version 0.0.1 Class create
 */
class Level {
private:

	/**
	 * 3D Tilearray [0=bg,1=main,2=fg][x][y]
	 */
	Tile ****tilelist;
	/**
	 * the name of the Level.
	 */
	string name;
	SDL_Surface *background;
	b2World *world;
	Entity* player; 
	int width;
	int height;
	float gravity;
	int time;
	b2Vec2 *gravity2d;

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


	Entity* getPlayer() {
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

	Tile**** getTilelist() const
	{
		return tilelist;
	}

	SDL_Surface* getBackground() const
	{
		return background;
	}

	b2World* getWorld() const
	{
		return world;
	}
};


#endif
