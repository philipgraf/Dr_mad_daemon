/*
 * PDA.cpp
 *
 *  Created on: 19.02.2013
 *      Author: philip
 */

#include <iostream>

#include "PDA.h"
#include "define.h"
#include "Game.h"

#include "SDL/SDL_ttf.h"

#include <string>

using namespace std;

PDA::PDA(int level) {

	this->level = level;
	running = true;
	green.g = 255;
	green.r = 0;
	green.b = 0;
	string filename;
	currentItem =0;

	switch (level) {
	case PDA_CLOCK:
		filename = PDAIMG"clock.bmp";
		break;
	default:
		filename = PDAIMG"clock.bmp";
		break;
	}

	SDL_Surface *temp = SDL_LoadBMP(filename.c_str());
	if (temp == NULL) {
		//TODO Throw exception
		cout << "unable to load " << filename << endl;
	} else {
		image = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		if (image != 0) {
			SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(image->format, 255, 0, 255));
		}
	}
	curser = TTF_RenderUTF8_Blended(Game::curGame->getFont(FONT_PDA_CLOCK), ">", green);

	SDL_Surface *screen = SDL_GetVideoSurface();

	display = SDL_CreateRGBSurface(SDL_HWSURFACE, 315, 290, SDL_GetVideoInfo()->vfmt->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);

}

PDA::~PDA() {
	SDL_FreeSurface(image);
	SDL_FreeSurface(display);
}

int PDA::show() {
	SDL_Event event;

	SDL_Rect displayRect;
	displayRect.x = 30;
	displayRect.y = 150;
	displayRect.w = display->w;
	displayRect.h = display->h;

	SDL_Rect imageRect;
	imageRect.x = 100;
	imageRect.y = 40;
	imageRect.w = image->w;
	imageRect.h = image->h;

	SDL_Surface *text = TTF_RenderUTF8_Blended(Game::curGame->getFont(FONT_PDA_CLOCK), "Test text", green);

	SDL_Rect textRect;
	textRect.x = 10;
	textRect.y = 0;
	textRect.w = text->w;
	textRect.h = text->h;

	SDL_Rect curserRect;
	curserRect.x = 0;
	curserRect.y = 0;
	curserRect.w = curser->w;
	curserRect.h = curser->h;

	running = true;

	while (running) {
		while (SDL_PollEvent(&event)) {
			onEvent(&event);
		}

		SDL_BlitSurface(curser, NULL, display, &curserRect);
		SDL_BlitSurface(text, NULL, display, &textRect);

		SDL_BlitSurface(display, NULL, image, &displayRect);
		SDL_BlitSurface(image, NULL, SDL_GetVideoSurface(), &imageRect);

		SDL_Flip(SDL_GetVideoSurface());
	}

	return 0;

}

void PDA::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch (sym) {
	case SDLK_ESCAPE:
	case SDLK_p:
		running = false;
		break;
	}
}

