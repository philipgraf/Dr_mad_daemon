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
#include <map>
#include <sstream>

using namespace std;

PDA::PDA(int level) {

	this->level = level;
	running = true;
	green.g = 255;
	green.r = 0;
	green.b = 0;
	string filename;
	currentItem = 0;

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

	displayRect.x = 30;
	displayRect.y = 150;
	displayRect.w = display->w;
	displayRect.h = display->h;

	imageRect.x = 100;
	imageRect.y = 40;
	imageRect.w = image->w;
	imageRect.h = image->h;

	curserRect.x = 0;
	curserRect.y = 0;
	curserRect.w = curser->w;
	curserRect.h = curser->h;
}

PDA::~PDA() {
	SDL_FreeSurface(image);
	SDL_FreeSurface(display);
	SDL_FreeSurface(curser);
	for (int i = 0; i < itemlist.size(); i++) {
		SDL_FreeSurface(itemlist[i].amound);
		SDL_FreeSurface(itemlist[i].itemname);
	}
	itemlist.clear();
}

int PDA::show() {
	SDL_Event event;

	build();

	running = true;
	Uint32 start;
	while (running) {
		start = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			onEvent(&event);
		}

		render();

		if (SDL_GetTicks() - start < 1000 / FPS) {
			SDL_Delay(1000 / FPS - (SDL_GetTicks() - start));
		}
	}

	return 0;

}

void PDA::render() {
	SDL_FillRect(display, &display->clip_rect, SDL_MapRGB(display->format, 0, 0, 0));
	if (itemlist.size() != 0) {
		curserRect.y = (curser->h + 2) * currentItem;
		SDL_BlitSurface(curser, NULL, display, &curserRect);
	} else {
		SDL_BlitSurface(TTF_RenderUTF8_Blended(Game::curGame->getFont(FONT_PDA_CLOCK), "Oh Boy  you have no items", green), NULL, display, NULL);
	}

	for (int i = 0; i < itemlist.size(); i++) {
		SDL_Rect labelRect;
		labelRect.x = 15;
		labelRect.y = (itemlist[i].itemname->h + 2) * i;
		labelRect.w = itemlist[i].itemname->w;
		labelRect.h = itemlist[i].itemname->h;
		SDL_BlitSurface(itemlist[i].itemname, NULL, display, &labelRect);
		labelRect.x = 280;
		SDL_BlitSurface(itemlist[i].amound, NULL, display, &labelRect);
	}

	SDL_BlitSurface(display, NULL, image, &displayRect);
	SDL_BlitSurface(image, NULL, SDL_GetVideoSurface(), &imageRect);
	SDL_Flip(SDL_GetVideoSurface());
}

void PDA::build() {
	map<string, int> playerItems = Game::curGame->getCurrentLevel()->getPlayer()->getItems();

	TTF_Font *font = Game::curGame->getFont(FONT_PDA_CLOCK);

	for (int i = 0; i < itemlist.size(); i++) {
		SDL_FreeSurface(itemlist[i].amound);
		SDL_FreeSurface(itemlist[i].itemname);
	}
	itemlist.clear();

	for (map<string, int>::iterator it = playerItems.begin(); it != playerItems.end(); ++it) {
		items_t item;
		item.itemname = TTF_RenderUTF8_Blended(font, lang[(*it).first].c_str(), green);
		stringstream s;
		s << (*it).second;
		item.amound = TTF_RenderUTF8_Blended(font, s.str().c_str(), green);
		itemlist.push_back(item);
	}
}

void PDA::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch (sym) {
	case SDLK_ESCAPE:
	case SDLK_p:
		running = false;
		break;
	case SDLK_DOWN:
		currentItem = (currentItem >= itemlist.size() - 1) ? itemlist.size() - 1 : currentItem + 1;
		break;
	case SDLK_UP:
		currentItem = (currentItem <= 0) ? 0 : (currentItem - 1);
		break;
	}
}

