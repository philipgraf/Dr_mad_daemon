#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <string>
#include <vector>
#include <map>
#include <box2d/box2d.h>
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
 * @author Felix Eckner
 * @date 14.04.2013
 * @version 0.1.0 Alpha-State
 */
class Level: public Event {
public:
	static vector<string> levels; /**< list of all level in the right order */
	static map<string, string> levelnames; /**< a map with the level and the levelname, needed for the levelmenu */
private:

	/**
	 * 3D Tilearray [0=bg,1=main,2=fg][x][y]
	 */
	Tile ****tilelist; /**< 3 dimensional array with all tiles */

	int levelnum; /**< the level number */
	string name; /**< the name of the Level. */
	SDL_Surface *bgImage; /**< the background image of the Level */
	Mix_Music *bgMusic; /**< the background music of the Level */
	b2World *world; /**< the collision world where all the basic collision detection happens */
	Player* player; /**< an object of the player */
	int width; /**< the width of the level in meter */
	int height; /**< the height of the level in meter */
	int time; /**< the time in seconds the player have to complete the level */
	b2Vec2 *gravity2d; /**< the gravity vector */
	Camera* mainCam; /**< the main camera of the level */
	Uint8 switches; /**< 8 switches of the level. All switches must be set to 1 to complete the level*/
	bool running; /**< while this is true the level will be rendered */
	bool levelFinished; /**< contains true if the level is finished and false if the player is dead */
	Language lang;  /**< this objekt is used for translation the notifications*/

	Uint32 timer; /**< contains the time in milliseconds which is needed for the time */

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

	void render();

	void logic();

	void play();

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
