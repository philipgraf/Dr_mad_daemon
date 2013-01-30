/*
 * Camera.cpp
 *
 *  Created on: 03.01.2013
 *      Author: philip
 */

#include "Camera.h"
/**
 * Constuctor.
 * Set the cameraMode to STICKY and target, width, height to the given values
 * @param target The entity which the camera will focus
 * @param w the width of the camera
 * @param h the height of the camera
 */
Camera::Camera(Entity* target, int w, int h) {
	this->target = target;
	width = w;
	height = h;
	cameraMode = STICKY;
	if (target != NULL) {
		// Center camera on target.
		x = target->getX() * TILESIZE - w / 2;
		y = target->getY() * TILESIZE - h / 2;
	}

}

/** constuctor
 * Set the cameraMode to CUSTOM and position and size to given values.
 * @param x The x position of the camera
 * @param y The y position of the camera
 * @param w The width of the camera
 * @parma h The Height of the camera
 */
Camera::Camera(float x, float y, int w, int h) {
	this->target = NULL;
	this->x = (int) x * TILESIZE;
	this->y = (int) y * TILESIZE;
	width = w;
	height = h;
	cameraMode = CUSTOM;

}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

void Camera::logic() {

	if (cameraMode == STICKY) {
		x = target->getX() * TILESIZE - width / 2;
		y = target->getY() * TILESIZE - height / 2;
	} else if (cameraMode == CUSTOM) {

	}

	if (x < 0) {
		x = 0;
	}
	if (y < 0) {
		y = 0;
	}

	if (x + width > (Game::curGame->getCurrentLevel()->getWidth()) * TILESIZE) {
		x = (Game::curGame->getCurrentLevel()->getWidth()) * TILESIZE - width;
	}

	if (y + height > (Game::curGame->getCurrentLevel()->getHeight()) * TILESIZE) {
		y = Game::curGame->getCurrentLevel()->getHeight() * TILESIZE - height;
	}

}

void Camera::drawImage() {
	drawBackground();
	drawTiles(BACKGROUND);
	drawEntities();
	drawTiles(MAIN);
	drawTiles(FOREGROUND);
}

void Camera::drawEntities() {

	for (std::vector<Entity*>::iterator it = Entity::entityList.begin();
			it != Entity::entityList.end(); ++it) {

		Entity* curEntity = *it;

		if (curEntity->isAlive()) {
			// if entcurEntityy left of camera
			if (curEntity->getX() * TILESIZE
					+ curEntity->getImage()->clip_rect.w / 2 < x) {
				continue;
			}
			// if entcurEntityy right of camera
			if (curEntity->getX() * TILESIZE > x + width) {
				continue;
			}
			// if entcurEntityy on top of camera
			if (curEntity->getY() * TILESIZE
					+ curEntity->getImage()->clip_rect.h < y) {
				continue;
			}
			// if entcurEntityy below bottom of camera
			if (curEntity->getY() * TILESIZE > y + height) {
				continue;
			}

			SDL_Rect destRect;
			destRect.x = (curEntity->getX() - curEntity->getWidth() / 2)
					* TILESIZE - x;
			destRect.y = (curEntity->getY() - curEntity->getHeight() / 2)
					* TILESIZE - y;
			destRect.w = curEntity->getWidth() * TILESIZE;
			destRect.h = curEntity->getHeight() * TILESIZE;

			SDL_Rect srcRect = curEntity->getCurFrameRect();

			// TODO: curEntity correct Frame for animation
			SDL_BlitSurface(curEntity->getImage(), &srcRect,
					SDL_GetVideoSurface(), &destRect);
		}
	}
}

void Camera::drawTiles(int layer) {
	//get shortcut to the level
	Level* curLevel = Game::curGame->getCurrentLevel();
	Tile ****tilelist = curLevel->getTilelist();

	//get and check tilerange
	int minY = this->y / TILESIZE;
	int maxY = (this->y + height) / TILESIZE;
	if (maxY >= curLevel->getHeight()) {
		maxY = curLevel->getHeight() - 1;
	}

	int minX = this->x / TILESIZE;
	int maxX = (this->x + width) / TILESIZE;
	if (maxX >= curLevel->getWidth()) {
		maxX = curLevel->getWidth() - 1;
	}

	//add selected tiles to screen
	for (int y = minY; y <= maxY; y++) {
		for (int x = minX; x <= maxX; x++) {
			u_int16_t id = tilelist[layer][x][y]->getId();
			int currentframe = tilelist[layer][x][y]->getCurrentframe();

			SDL_Rect srcRect = { currentframe * TILESIZE, id * TILESIZE,
					TILESIZE, TILESIZE };

			SDL_Rect destRect = { x * TILESIZE - this->x, y * TILESIZE
					- this->y, TILESIZE, TILESIZE };

			SDL_BlitSurface(Tile::tileset, &srcRect, SDL_GetVideoSurface(),
					&destRect);
		}
	}

}

void Camera::drawBackground() {
	Level *curLevel = Game::curGame->getCurrentLevel();
	int bgWidth = curLevel->getBackground()->clip_rect.w;
	int bgHeight = curLevel->getBackground()->clip_rect.h;
	int lWidth = curLevel->getWidth() * TILESIZE;
	int lHeight = curLevel->getHeight() * TILESIZE;

	if (cameraMode == STICKY) {
		// center background behind level and move it with half speed
		SDL_Rect destRect = { lWidth / 2 - bgWidth / 2 - x / 2, lHeight / 2
				- bgHeight / 2 - y / 2, curLevel->getBackground()->clip_rect.w,
				curLevel->getBackground()->clip_rect.h };
		SDL_BlitSurface(curLevel->getBackground(), NULL, SDL_GetVideoSurface(),
				&destRect);
	} else {
		Tools::drawImage(SDL_GetVideoSurface(), 0, 0,
				Game::curGame->getCurrentLevel()->getBackground(), x, y, width,
				height);
	}
}

void Camera::setPosition(int x, int y) {
	this->x = x - width / 2;
	this->y = y - height / 2;
}

/**
 * Set the camera position depends on given Target.
 * @params target Entity which will be centered
 */
void Camera::setPosition(Entity* target) {
	x = (target->getX() + target->getImage()->clip_rect.w / 2) - width / 2;
	y = (target->getY() + target->getImage()->clip_rect.h / 2) - height / 2;
}

/**
 * Move camera.
 *
 * @param h Horizontal movement
 * @param v Vertical movement
 */
void Camera::move(int h, int v) {
	x += h;
	y += v;
}

/**
 * Get the current Camera Mode.
 * @return cameraMode
 */
Camera::mode Camera::getCameraMode() {
	return cameraMode;
}

/**
 * Set the current Camera Mode with given value.
 *@param cameraMode Mode that will be set
 */
void Camera::setCameraMode(mode cameraMode) {
	this->cameraMode = cameraMode;
}

/**
 * Get the Height of the camera
 * @return the height of the camera
 */
int Camera::getHeight() const {
	return height;
}

/**
 * Set the Height to the given value.
 * @param height value which the height will be set to.
 */
void Camera::setHeight(int height) {
	this->height = height;
}

const Entity* Camera::getTarget() const {
	return target;
}

void Camera::setTarget(Entity* target) {
	this->target = target;
}

int Camera::getWidth() const {
	return width;
}

void Camera::setWidth(int width) {
	this->width = width;
}

int Camera::getX() const {
	return x;
}

int Camera::getY() const {
	return y;
}

SDL_Rect Camera::getRect() {
	SDL_Rect rec;
	rec.x = x;
	rec.y = y;
	rec.w = width;
	rec.h = height;
	return rec;
}
