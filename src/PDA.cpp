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

vector<map<string, int> > PDA::updateReqList;

PDA::PDA(int level) {

	this->level = level;
	running = false;
	green.g = 255;
	green.r = 0;
	green.b = 0;
	currentItem = 0;
	timer = NULL;
	image = NULL;
	lcd = NULL;

	curser = TTF_RenderUTF8_Blended(Game::curGame->getFont(FONT_PDA_CLOCK), ">", green);
	SDL_Surface *screen = SDL_GetVideoSurface();

	display = SDL_CreateRGBSurface(SDL_HWSURFACE, 250, 230, SDL_GetVideoInfo()->vfmt->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);

	displayRect.x = 32;
	displayRect.y = 123;
	displayRect.w = display->w;
	displayRect.h = display->h;

	curserRect.x = 0;
	curserRect.y = 0;
	curserRect.w = curser->w;
	curserRect.h = curser->h;

	updateText = TTF_RenderUTF8_Blended(Game::curGame->getFont(FONT_PDA_CLOCK_SMALL), "Press \"A\" to Update PDA", green);
	updateRequirements = NULL;

	init();

}

PDA::~PDA() {
	SDL_FreeSurface(image);
	SDL_FreeSurface(display);
	SDL_FreeSurface(curser);
	if (lcd != NULL) {
		SDL_FreeSurface(lcd);
	}
	for (unsigned i = 0; i < itemlist.size(); i++) {
		SDL_FreeSurface(itemlist[i].amound);
		SDL_FreeSurface(itemlist[i].itemname);
	}
	itemlist.clear();
}

void PDA::loadRequirements() {

	map<string, int> req;

	//Level 1 Requirements
	req["screw"] = 4;
	req["inductor"] = 2;
	req["wire"] = 5;
	req["pcb"] = 2;
	req["slot"] = 1;
	updateReqList.push_back(req);

	req.clear();
	//Level 2 Requirements
	req["screw"] = 4;
	req["capacitor"] = 2;
	req["antimatter"] = 2;

	updateReqList.push_back(req);

}

int PDA::getLevel() const {
	return level;
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

void PDA::init() {

	if (image != NULL) {
		SDL_FreeSurface(image);
	}

	if (lcd != NULL) {
		SDL_FreeSurface(lcd);
	}

	string filename;
	switch (level) {
	case PDA_CLOCK:
		filename = PDAIMG"clock.bmp";
		break;
	case PDA_GLOVE:
		filename = PDAIMG"glove.bmp";
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
	imageRect.x = 16;
	imageRect.y = 40;
	imageRect.w = image->w;
	imageRect.h = image->h;
	SDL_Surface *screen = SDL_GetVideoSurface();

	if (level > 0) {
		lcd = SDL_CreateRGBSurface(SDL_HWSURFACE, 240, 160, SDL_GetVideoInfo()->vfmt->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
	} else {
		lcd = NULL;
	}

	if (lcd != NULL) {
		lcdRect.x = 358;
		lcdRect.y = 137;
		lcdRect.w = lcd->w;
		lcdRect.h = lcd->h;
	}

	stringstream reqString;
	for (map<string, int>::iterator it = updateReqList[level].begin(); it != updateReqList[level].end(); ++it) {
		reqString << it->first << ": " << it->second << ", ";
	}
	updateRequirements = TTF_RenderUTF8_Blended(Game::curGame->getFont(FONT_PDA_CLOCK_SMALL), reqString.str().c_str(), green);

	updateTextRect.x = 0;
	updateTextRect.y = displayRect.h - updateText->clip_rect.h - updateRequirements->clip_rect.h;
	updateTextRect.h = updateText->clip_rect.h;
	updateTextRect.w = updateText->clip_rect.w;

	updateRequirementsRect.x = 0;
	updateRequirementsRect.y = displayRect.h - updateRequirements->clip_rect.h;
	updateRequirementsRect.h = updateRequirements->clip_rect.h;
	updateRequirementsRect.w = updateRequirements->clip_rect.w;

}

void PDA::render() {
	SDL_FillRect(display, &display->clip_rect, SDL_MapRGB(display->format, 0, 0, 0));

	SDL_BlitSurface(timer, NULL, display, &timerRect);

	if (itemlist.size() != 0) {
		// 20 pixel per line plus 50 pixel for the timer
		curserRect.y = 20 * currentItem + 50;
		SDL_BlitSurface(curser, NULL, display, &curserRect);
	} else {
		SDL_Rect textRect;
		textRect.x = 0;
		textRect.y = 50;
		textRect.h = 20;
		textRect.w = display->w;
		SDL_BlitSurface(TTF_RenderUTF8_Blended(Game::curGame->getFont(FONT_PDA_CLOCK), "Oh No Boy!! You have no items", green), NULL, display, &textRect);
	}

	for (unsigned i = 0; i < itemlist.size(); i++) {
		SDL_Rect labelRect;
		labelRect.x = 15;
		labelRect.y = 20 * i + 50;
		labelRect.w = itemlist[i].itemname->w;
		labelRect.h = itemlist[i].itemname->h;
		SDL_BlitSurface(itemlist[i].itemname, NULL, display, &labelRect);
		labelRect.x = displayRect.w - itemlist[i].amound->w;
		SDL_BlitSurface(itemlist[i].amound, NULL, display, &labelRect);
	}

	SDL_BlitSurface(updateText, NULL, display, &updateTextRect);
	SDL_BlitSurface(updateRequirements, NULL, display, &updateRequirementsRect);

	SDL_BlitSurface(display, NULL, image, &displayRect);
	if (lcd != NULL) {
		SDL_BlitSurface(lcd, NULL, image, &lcdRect);
	}
	SDL_BlitSurface(image, NULL, SDL_GetVideoSurface(), &imageRect);
	SDL_Flip(SDL_GetVideoSurface());
}

void PDA::build() {
	map<string, int> playerItems = Game::curGame->getCurrentLevel()->getPlayer()->getItems();

	TTF_Font *font = Game::curGame->getFont(FONT_PDA_CLOCK);

	for (unsigned i = 0; i < itemlist.size(); i++) {
		SDL_FreeSurface(itemlist[i].amound);
		SDL_FreeSurface(itemlist[i].itemname);
	}
	itemlist.clear();

	stringstream s;
	s << Game::curGame->getCurrentLevel()->getTime();

	if (timer != NULL) {
		SDL_FreeSurface(timer);
	}

	timer = TTF_RenderUTF8_Blended(Game::curGame->getFont(FONT_PDA_CLOCK_TIMER), s.str().c_str(), green);

	timerRect.y = 0;
	timerRect.x = display->w - timer->w;
	timerRect.h = timer->h;
	timerRect.w = timer->w;

	for (map<string, int>::iterator it = playerItems.begin(); it != playerItems.end(); ++it) {
		if (it->second > 0) {
			items_t item;
			item.itemname = TTF_RenderUTF8_Blended(font, lang[(*it).first].c_str(), green);
			stringstream s;
			s << it->second;
			item.amound = TTF_RenderUTF8_Blended(font, s.str().c_str(), green);
			itemlist.push_back(item);
		}
	}
}

void PDA::update() {
	map<string, int> &playerItems = Game::curGame->getCurrentLevel()->getPlayer()->getItems();

	bool itemsOk = true;

	for (map<string, int>::iterator it = updateReqList[level].begin(); it != updateReqList[level].end(); ++it) {
		if (playerItems[it->first] < it->second) {
			itemsOk = false;
			break;
		}
	}

	if (itemsOk) {
		cout << "test" << endl;
		for (map<string, int>::iterator it = updateReqList[level].begin(); it != updateReqList[level].end(); ++it) {
			playerItems[it->first] -= it->second;
		}
		level++;
		init();
		build();
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
	case SDLK_a:
		update();
		break;
	default:
		break;
	}
}

void PDA::onWiiButtonEvent(int button) {
	if (button & WII_BTN_LEFT) {
		currentItem = (currentItem >= itemlist.size() - 1) ? itemlist.size() - 1 : currentItem + 1;
	}

	if (button & WII_BTN_RIGHT) {
		currentItem = (currentItem <= 0) ? 0 : (currentItem - 1);
	}

	if ((button & WII_BTN_1) || (button & WII_BTN_PLUS)) {
		running = false;
	}
	if(button & WII_BTN_A){
		update();
	}
}

