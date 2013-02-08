#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <string>
#include <fstream>
#include <Box2D/Box2D.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <yaml-cpp/yaml.h>

#include "Tile.h"
#include "Entity.h"
#include "Camera.h"
#include "Game.h"
#include "Debug.h"
#include "Event.h"

using namespace std;

class Entity;
class Camera;

/** Level class.
 * This class contain all Tiles, the name, the Background Image, the Player, absolute width and height in Tiles, the gravity and the time.
 * All values loaded from file.
 * @author Philip Graf
 * @date 04.11.2012
 * @version 0.0.1 Class create
 */
class Level : public Event {
private:

	/**
	 * 3D Tilearray [0=bg,1=main,2=fg][x][y]
	 */
	Tile ****tilelist;

	string name; /**< the name of the Level. */
	SDL_Surface *bgImage;
	Mix_Music *bgMusic;
	b2World *world;
	Entity* player; 
	int width;
	int height;
	int time;
	b2Vec2 *gravity2d;
	Camera* mainCam;
	bool running;

#ifdef DEBUG
	Debug b2Debug;
#endif



	void loadMapFile(string filename);
	void onKeyUP(SDLKey sym, SDLMod mod, Uint16 unicode) ;
	void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	
public:
	Level(string lname="l000");
	~Level();

	void render();
	void logic();
	void play();



	/*********************** GETTER / SETTER ************************/
	int getGravity() const;
	void setGravity(int gravity);
	const string& getName() const;
	int getTime() const;
	void setTime(int time);
	int getTileID(int x,int y,int layer=1);
	Entity* getPlayer();
	int getHeight() const;
	int getWidth() const;
	Tile**** getTilelist() const;
	SDL_Surface* getBackground() const;
	b2World* getWorld() const;
	void setRunning(bool running);
};


#endif
