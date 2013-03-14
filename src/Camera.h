#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL/SDL.h>

#include "Entity.h"
#include "define.h"

#define	BACKGROUND 0
#define	MAIN 1
#define	FOREGROUND 2

class Entity;

class Camera {
public:
	/** Cameramode enum.
	 *	manage the main properties of the camera
	 */
	enum {
		STICKY = 0, /**< the camera will center the entity stored in the target variable */
		SMOOTH, /**< the camera will follow the entity smoothly (not implemented yet) */
		CUSTOM /**< the camera will stay on the x, y position */
	}typedef mode;
private:

	int x; /**< The absolute x value of the camera. */
	int y; /**< The absolute y value of the camera */
	int width; /**< The camera width */
	int height; /**< The camera height */
	Entity *target; /**< This contain an Entity if cameraMode is STICKY and SMOOTH */
	mode cameraMode; /**< cameraMode of this camera */
	SDL_Surface *crosshairs; /**< crosshairs which will be drawn over the selected Entity if the player have one */


	void drawItems();
	void drawNotification();
	void drawEntities();
	void drawTiles(int layer);
	void drawBackground();
public:

	Camera(Entity* target, int w = WIDTH, int h = HEIGHT);
	Camera(float x = 0, float y = 0, int w = WIDTH, int h = HEIGHT);
	virtual ~Camera();

	void logic();
	void drawImage();
	void move(int h, int v);

	void setPosition(int x, int y);
	void setPosition(Entity* target);
	mode getCameraMode();
	void setCameraMode(mode cameraMode);
	int getHeight() const;
	void setHeight(int height);
	const Entity* getTarget() const;
	void setTarget(Entity* target);
	int getWidth() const;
	void setWidth(int width);
	int getX() const;
	int getY() const;
	SDL_Rect getRect();

};

#endif /* CAMERA_H_ */
