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
#include "Language.h"

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
	Uint8 switches;
	bool running; /**< while this is true the level will be rendered */
	bool levelFinished;
	Language lang;

	Uint32 timer;

#if DEBUG >= 3
	Debug b2Debug;
#endif

	void loadMapFile(string filename);
	void onKeyUP(SDLKey sym, SDLMod mod, Uint16 unicode);
	void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void onWiiButtonEvent(int button);
	void updateTime();

	void level0Logic();
	void level1Logic();
	void level2Logic();
	void level3Logic();

public:
	static void loadLevels();

	Level(unsigned levelnum);
	~Level();

	/** Render function from level.
	 *	tell the main camera to render the level and flip the surface
	 */
	void render();

	/** Logic function of the level.
	 * update the time, call all tile, entity, items, notification and camera logic.
	 * also check if the player is alive and if the level is finished.
	 */
	void logic();

	/** Play function of the level.
	 * the main loop which runs until running is false.
	 * @see running()
	 */
	void play();

	/** Call the right level-Switch logic
	 * call the level logic depend on the levelnumber.
	 * @see level0logic()
	 * @see level1logic()
	 * @see level2logic()
	 * @see level3logic()
	 * @see levelnum()
	 */
	void switchActions();

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
	void toggleSwitch(Uint8 flags);
	Uint8 getSwitches() const;
};

#endif
