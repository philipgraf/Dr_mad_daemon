/*
 * Surface.cpp
 *
 *  Created on: 27.09.2012
 *      Author: medi
 */

#include "Tools.h"

void Tools::error(string msg) {

	cout << "Error: " << msg << endl;

	exit(1);
}

SDL_Surface * Tools::loadImage(string filename) {
	SDL_Surface *temp, *ret;
	if ((temp = IMG_Load(filename.c_str())) == NULL) {
		Tools::error("Unable to load Image: " + filename);
	}
	ret = SDL_DisplayFormatAlpha(temp);
	SDL_FreeSurface(temp);
	return ret;
}

// TODO: deprecated
bool Tools::drawImage(SDL_Surface* dstSurface, int dstX, int dstY,
		SDL_Surface* srcSurface, int srcX, int srcY, int width, int height) {
	if (dstSurface == NULL || srcSurface == NULL) {
		Tools::error("drawImage: Surface not defined");
		return false;
	}
	SDL_Rect dstRect;
	dstRect.x = dstX;
	dstRect.y = dstY;

	SDL_Rect srcRect;
	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = width;
	srcRect.h = height;
	SDL_BlitSurface(srcSurface, &srcRect, dstSurface, &dstRect);
	return true;
}
