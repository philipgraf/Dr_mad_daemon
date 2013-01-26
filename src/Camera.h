/*
 * Camera.h
 *
 *  Created on: 03.01.2013
 *      Author: philip
 */

#include "includes.h"

#ifndef CAMERA_H_
#define CAMERA_H_


#define	BACKGROUND 0
#define	MAIN 1
#define	FOREGROUND 2

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
	/**
	 * The absolute x value of the camera.
	 */
	int x;
	/**
	 *  The absolute y value of the camera
	 */
	int y;
	int width; /**< The camera width */
	int height; /**< The camera height */
	Entity *target; /**< This contain an Entity if cameraMODE is STICKY and SMOOTH */
	mode cameraMode;

	void drawEntities();
	void drawTiles(int layer);
	void drawBackground();

public:
	/**
	 * Constuctor.
	 * Set the cameraMode to STICKY and target, width, height to the given values
	 * @param target The entity which the camera will focus
	 * @param w the width of the camera
	 * @param h the height of the camera
	 */
	Camera(Entity* target, int w=WIDTH, int h=HEIGHT);

	/** constuctor
	 * Set the cameraMode to CUSTOM and position and size to given values.
	 * @param x The x position of the camera
	 * @param y The y position of the camera
	 * @param w The width of the camera
	 * @parma h The Height of the camera
	 */
	Camera(int x=0,int y=0, int w=WIDTH, int h=HEIGHT);
	virtual ~Camera();

	void logic();

	void drawImage();

	void setPosition(int x,int y){
		this->x=x-width/2;
		this->y=y-height/2;
	}

	/**
	 * Set the camera position depends on given Target.
	 * @params target Entity which will be centered
	 */
	void setPosition(Entity* target){
		x=(target->getX()+target->getImage()->clip_rect.w/2)-width/2;
		y=(target->getY()+target->getImage()->clip_rect.h/2)-height/2;
	}

	/**
	 * Move camera.
	 *
	 * @param h Horizontal movement
	 * @param v Vertical movement
	 */
	void move(int h,int v){
		x+=h;
		y+=v;
	}

	/**
	 * Get the current Camera Mode.
	 * @return cameraMode
	 */
	mode getCameraMode() const {
		return cameraMode;
	}

	/**
	 * Set the current Camera Mode with given value.
	 *@param cameraMode Mode that will be set
	 */
	void setCameraMode(mode cameraMode) {
		this->cameraMode = cameraMode;
	}

	/**
	 * Get the Height of the camera
	 * @return the height of the camera
	 */
	int getHeight() const {
		return height;
	}

	/**
	 * Set the Height to the given value.
	 * @param height value which the height will be set to.
	 */
	void setHeight(int height) {
		this->height = height;
	}

	const Entity* getTarget() const {
		return target;
	}

	void setTarget(Entity* target) {
		this->target = target;
	}

	int getWidth() const {
		return width;
	}

	void setWidth(int width) {
		this->width = width;
	}

	int getX() const {
		return x;
	}


	int getY() const {
		return y;
	}

	SDL_Rect getRect(){
		SDL_Rect rec;
		rec.x=x;
		rec.y=y;
		rec.w=width;
		rec.h=height;
		return rec;
	}

};

#endif /* CAMERA_H_ */
