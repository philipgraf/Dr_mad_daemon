/*
 * Camera.h
 *
 *  Created on: 03.01.2013
 *      Author: philip
 */

#include "includes.h"

#ifndef CAMERA_H_
#define CAMERA_H_

class Camera {
public:
	enum{
			STICKY=0,
			SMOOTH,
			CUSTOM
		}typedef mode;
private:
	int x;
	int y;
	int width;
	int height;
	Entity *target;
	mode cameraMode;

public:
	Camera(Entity* target, int w=WIDTH, int h=HEIGHT);
	Camera(int x=0,int y=0, int w=WIDTH, int h=HEIGHT);
	virtual ~Camera();

	void logic();

	void drawImage();

	void setPosition(int x,int y){
		this->x=x-width/2;
		this->y=y-height/2;
	}

	/**
	 * Set the camera position depends on given Target
	 * @params target Entity which will be centered
	 */
	void setPosition(Entity* target){
		x=(target->getX()+target->getImage()->clip_rect.w/2)-width/2;
		y=(target->getY()+target->getImage()->clip_rect.h/2)-height/2;
	}

	/**
	 * Move camera
	 *
	 * @params h Horizontal movement
	 * @params v Vertical movement
	 */
	void move(int h,int v){
		x+=h;
		y+=v;
	}

	/**
	 * Get the current Camera Mode
	 * @return cameraMode
	 */
	mode getCameraMode() const {
		return cameraMode;
	}

	void setCameraMode(mode cameraMode) {
		this->cameraMode = cameraMode;
	}

	int getHeight() const {
		return height;
	}

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
