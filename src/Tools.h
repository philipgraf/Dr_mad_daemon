/*
 * Surface.h
 *
 *  Created on: 27.09.2012
 *      Author: medi
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include "includes.h"

using namespace std;

class Tools {
public:

	static void error(string msg);
	static SDL_Surface* loadImage(string filename);
	static bool drawImage(SDL_Surface* dstSurface,int dstX,int dstY,SDL_Surface* srcSurface,int srcX=0, int srcY=0,int width=TILESIZE,int height=TILESIZE);
};

#endif /* TOOLS_H_ */
