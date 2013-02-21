#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <string>
#include <vector>
#include <map>
#include <Box2D/Box2D.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "Tile.h"
#include "Debug.h"
#include "Player.h"
#include "Camera.h"
#include "Event.h"
#include "BadGuy.h"

using namespace std;

class Player;
class Camera;

/** Level class.
 * This class contain all Tiles, the name, the Background Image, the Player, absolute width and height in Tiles, the gravity and the time.
 * All values loaded from file.
 * @author Philip Graf
 * @date 04.11.2012
 * @version 0.0.1 Class create
 */
class Level: public Event {
public:
	static vector<string> levels;
	static map<string, string> levelnames;
private:

	/**
	 * 3D Tilearray [0=bg,1=main,2=fg][x][y]
	 */
	Tile ****tilelist;

	int levelnum;
	string name; /**< the name of the Level. */
	SDL_Surface *bgImage;
	Mix_Music *bgMusic;
	b2World *world;
	Player* player;
	int width;
	int height;
	int time;
	b2Vec2 *gravity2d;
	Camera* mainCam;
	bool running;
	bool levelFinished;

	Uint32 timer;

#if DEBUG >= 3
	Debug b2Debug;
#endif

	void loadMapFile(string filename);
	void onKeyUP(SDLKey sym, SDLMod mod, Uint16 unicode);
	void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void updateTime();

public:
	static void loadLevels();

	Level(unsigned levelnum);
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
	int getTileID(int x, int y, int layer = 1);
	Player* getPlayer();
	int getHeight() const;
	int getWidth() const;
	Tile**** getTilelist() const;
	SDL_Surface* getBackground() const;
	b2World* getWorld() const;
	void setRunning(bool running);
	bool isFinished() const;
	void setFinished(bool finished);
};

#endif
