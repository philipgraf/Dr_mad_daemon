/*
 * Camera.h
 *
 *  Created on: 03.01.2013
 *      Author: philip
 */
#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL/SDL.h>

#include "Entity.h"

#define	BACKGROUND 0
#define	MAIN 1
#define	FOREGROUND 2

class Entity;

/**
 * Camera Class.
 */
class Camera {
public:
	enum{
			STICKY=0,
			SMOOTH,
			CUSTOM
		}typedef mode;
private:

	int x; /**< The absolute x value of the camera. */
	int y; /**<The absolute y value of the camera */
	int width; /**< The camera width */
	int height; /**< The camera height */
	Entity *target; /**< This contain an Entity if cameraMODE is STICKY and SMOOTH */
	mode cameraMode;

	void drawEntities();
	void drawTiles(int layer);
	void drawBackground();
public:

	Camera(Entity* target, int w=WIDTH, int h=HEIGHT);
	Camera(float x=0,float y=0, int w=WIDTH, int h=HEIGHT);
	virtual ~Camera();

	void logic();
	void drawImage();
	void setPosition(int x,int y);
	void setPosition(Entity* target);
	void move(int h,int v);

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
