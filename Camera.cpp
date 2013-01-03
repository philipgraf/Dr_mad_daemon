/*
 * Camera.cpp
 *
 *  Created on: 03.01.2013
 *      Author: philip
 */

#include "Camera.h"


Camera::Camera(Entity* target, int w, int h) {
	this->target = target;
	width=w;
	height=h;
	cameraMode=STICKY;
	if(target != NULL){
		// Center camera on target.
		x=(target->getX()+target->getImage()->clip_rect.w/2)-w/2;
		y=(target->getY()+target->getImage()->clip_rect.h/2)-h/2;
	}
}

Camera::Camera(int x, int y, int w, int h) {
	this->target=NULL;

	this->x=x;
	this->y=y;
	width=w;
	height=h;
	cameraMode=CUSTOM;
}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

void Camera::logic() {

	if(cameraMode == STICKY){
		x=(target->getX()+target->getImage()->clip_rect.w/2)-width/2;
		y=(target->getY()+target->getImage()->clip_rect.h/2)-height/2;
	}

	if(x<0){
		x=0;
	}
	if(y<0){
		y=0;
	}
	cout << Game::curGame->getCurrentLevel()->getWidth() << endl;
	if(x+width > (Game::curGame->getCurrentLevel()->getWidth())*TILESIZE){
		x = (Game::curGame->getCurrentLevel()->getWidth())*TILESIZE - width;
	}

	if(y+height > (Game::curGame->getCurrentLevel()->getHeight())*TILESIZE){
		y = Game::curGame->getCurrentLevel()->getHeight()*TILESIZE - height;
	}

}

void Camera::drawImage() {
	SDL_Surface *screen = SDL_GetVideoSurface();

	for(std::vector<Entity*>::iterator it = Entity::entityList.begin(); it != Entity::entityList.end(); it++){

		Entity* curEntity = *it;

		if(curEntity->isAlive()){
			// if entcurEntityy left of camera
			if(curEntity->getX()+curEntity->getImage()->clip_rect.w<x){
				continue;
			}
			// if entcurEntityy right of camera
			if(curEntity->getX()>x+width){
				continue;
			}
			// if entcurEntityy on top of camera
			if(curEntity->getY()+curEntity->getImage()->clip_rect.h<y){
				continue;
			}
			// if entcurEntityy below bottom of camera
			if(curEntity->getY()>y+height){
				continue;
			}

			SDL_Rect destRect;
			destRect.x = curEntity->getX()-x;
			destRect.y = curEntity->getY()-y;
			destRect.w = curEntity->getWidth();
			destRect.h = curEntity->getHeight();

			SDL_Rect srcRect = curEntity->getCurFrameRect();


			// TODO: BlcurEntity correct Frame for animation
			SDL_BlitSurface(curEntity->getImage(),&srcRect,screen,&destRect);
		}

	}
}
