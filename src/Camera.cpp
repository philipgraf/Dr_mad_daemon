/*
 * Camera.cpp
 *
 *  Created on: 03.01.2013
 *      Author: philip
 */

#include "Camera.h"
#include "Game.h"
#include "Notification.h"
#include "Item.h"

using namespace std;

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

	SDL_Surface *tmp = SDL_LoadBMP(IMG"cross.bmp");
	if (tmp != NULL) {
		crosshairs = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		if (crosshairs != 0) {
			SDL_SetColorKey(crosshairs, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(crosshairs->format, 255, 0, 255));

		}
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
	SDL_Surface *tmp = SDL_LoadBMP(IMG"cross.bmp");
	if (tmp != NULL) {
		crosshairs = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		if (crosshairs != 0) {
			SDL_SetColorKey(crosshairs, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(crosshairs->format, 255, 0, 255));

		}
	}

}

/** Main logic of the Camera
 * If the mode is sticky the position will be recalculated from the Entity stored in the target variable
 * Checks the x and y value of the camera and set it back to zero if it is lower then zero and set it back to the maximum width.
 *
 * @see mode
 * @see target
 */
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

/**
 * call the draw functions in the correct order. From background to foreground.
 * @see drawBackground()
 * @see drawTiles()
 * @see drawEntities()
 * @see Items()
 * @see drawNotification()
 */
void Camera::drawImage() {
	drawBackground();
	drawTiles(BACKGROUND);
	drawEntities();
	drawTiles(MAIN);
	drawItems();
	drawTiles(FOREGROUND);
	drawNotification();
}

/**
 * draw all notifications stored in the notification list
 * @see Notification::notificationList
 */
void Camera::drawNotification() {
	int i = 0;
	for (vector<Notification*>::iterator it = Notification::notificationList.begin(); it != Notification::notificationList.end(); ++it) {
		SDL_Rect destRect;
		destRect.x = 10;
		destRect.y = i * 40;
		destRect.w = (*it)->getNotificationSurface()->clip_rect.w;
		destRect.h = (*it)->getNotificationSurface()->clip_rect.w;
		SDL_BlitSurface((*it)->getNotificationSurface(), NULL, SDL_GetVideoSurface(), &destRect);
		i++;
	}
}

/**
 * draw all items stored in the item list
 * @see Item::itemList
 */
void Camera::drawItems() {
	for (unsigned i = 0; i < Item::itemlist.size(); i++) {
		SDL_Rect destRect = Item::itemlist[i]->getClipRect();
		destRect.x = destRect.x - x;
		destRect.y = destRect.y - y;
		SDL_BlitSurface(Item::itemlist[i]->getImage(), NULL, SDL_GetVideoSurface(), &destRect);
	}
}

/**
 * draw all entities stored in the entity list
 * and put the crosshairs on it if it is selected
 * @see Entity::entityList
 * @see crosshairs
 */
void Camera::drawEntities() {

	for (vector<Entity*>::iterator it = Entity::entityList.begin(); it != Entity::entityList.end(); ++it) {

		Entity* curEntity = *it;

		// if entcurEntityy left of camera
		if (curEntity->getX() * TILESIZE + curEntity->getImage()->clip_rect.w / 2 < x) {
			continue;
		}
		// if entcurEntityy right of camera
		if (curEntity->getX() * TILESIZE > x + width) {
			continue;
		}
		// if entcurEntityy on top of camera
		if (curEntity->getY() * TILESIZE + curEntity->getImage()->clip_rect.h < y) {
			continue;
		}
		// if entcurEntityy below bottom of camera
		if (curEntity->getY() * TILESIZE > y + height) {
			continue;
		}

		SDL_Rect destRect;
		destRect.x = (curEntity->getX() - curEntity->getWidth() / 2) * TILESIZE - x;
		destRect.y = (curEntity->getY() - curEntity->getHeight() / 2) * TILESIZE - y;
		destRect.w = curEntity->getWidth() * TILESIZE;
		destRect.h = curEntity->getHeight() * TILESIZE;

		SDL_Rect srcRect = curEntity->getCurFrameRect();

		SDL_BlitSurface(curEntity->getImage(), &srcRect, SDL_GetVideoSurface(), &destRect);
	}
	if (Game::curGame->getCurrentLevel()->getPlayer()->getSelectedEntity() > 0) {
		Entity* selectedEntity = Entity::entityList[Game::curGame->getCurrentLevel()->getPlayer()->getSelectedEntity()];
		SDL_Rect destRect;
		destRect.x = (selectedEntity->getX() * TILESIZE) - crosshairs->w/2 - x;
		destRect.y = (selectedEntity->getY() * TILESIZE) - crosshairs->h/2 - y;
		destRect.w = selectedEntity->getWidth() * TILESIZE;
		destRect.h = selectedEntity->getHeight() * TILESIZE;

		SDL_BlitSurface(crosshairs, NULL, SDL_GetVideoSurface(), &destRect);
	}
}

/**
 * compute the viewable range of the litelist and draw the range of given layer on the screen. (Background, Main and Foreground)
 * @param layer contails the layer which will be rendered
 */
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

			SDL_Rect srcRect;
			srcRect.x = currentframe * TILESIZE;
			srcRect.y = id * TILESIZE;
			srcRect.w = TILESIZE;
			srcRect.h = TILESIZE;

			SDL_Rect destRect;
			destRect.x = x * TILESIZE - this->x;
			destRect.y = y * TILESIZE - this->y;
			destRect.w = TILESIZE;
			destRect.h = TILESIZE;

			SDL_BlitSurface(Tile::tileset, &srcRect, SDL_GetVideoSurface(), &destRect);
		}
	}

}

/**
 * draw the backgrounde image of the Level on the screen.
 */
void Camera::drawBackground() {
	Level *curLevel = Game::curGame->getCurrentLevel();
	int bgWidth = curLevel->getBackground()->clip_rect.w;
	int bgHeight = curLevel->getBackground()->clip_rect.h;
	int lWidth = curLevel->getWidth() * TILESIZE;
	int lHeight = curLevel->getHeight() * TILESIZE;

	if (cameraMode == STICKY) {
		// center background behind level and move it with half speed
		SDL_Rect destRect;
		destRect.x = lWidth / 2 - bgWidth / 2 - x / 2;
		destRect.y = lHeight / 2 - bgHeight / 2 - y / 2;
		destRect.w = curLevel->getBackground()->clip_rect.w;
		destRect.h = curLevel->getBackground()->clip_rect.h;

		SDL_BlitSurface(curLevel->getBackground(), NULL, SDL_GetVideoSurface(), &destRect);
	} else {
		SDL_BlitSurface(curLevel->getBackground(), NULL, SDL_GetVideoSurface(), NULL);
	}

}

/**
 * Set the position of the camera to given x and y values.
 *
 * @param x new x value
 * @param y new y value
 */
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
 * Increase the x and y of the camera by the given values.
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

/**
 * Get the target the camera have focused on.
 */
const Entity* Camera::getTarget() const {
	return target;
}

/**
 * Set the target of the camera
 */
void Camera::setTarget(Entity* target) {
	this->target = target;
}

/**
 * get the Width of the camera
 * @return the current camera height
 */
int Camera::getWidth() const {
	return width;
}

/**
 * Set the width of the camera to given value.
 * @param width the value camera will be set to.
 */
void Camera::setWidth(int width) {
	this->width = width;
}

/**
 * get the x position of the camera
 * @return the current x position
 */
int Camera::getX() const {
	return x;
}

/**
 * get the y position of the camera
 * @return the current y position
 */
int Camera::getY() const {
	return y;
}

/**
 * get the x,y position and the width and height as SDL_Rect
 * @return the whole camera proportion as SDL_Rect
 */
SDL_Rect Camera::getRect() {
	SDL_Rect rec;
	rec.x = x;
	rec.y = y;
	rec.w = width;
	rec.h = height;
	return rec;
}
