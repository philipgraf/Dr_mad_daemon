/*
 * PDA.h
 *
 *  Created on: 19.02.2013
 *      Author: philip
 */

#ifndef PDA_H_
#define PDA_H_

#define PDA_CLOCK 0
#define PDA_GLOVE 1

#include "Event.h"
#include "Language.h"

#include <SDL/SDL.h>
#include <vector>
#include <map>
#include <string>


typedef struct {
	SDL_Surface *itemname;
	SDL_Surface *amound;
} items_t;

/** PDA class.
 * This class handles the Player PDA
 * @author Felix Eckner
 * @date 14.03.2013
 * @version 0.1.0 Alpha-State
 */
class PDA: public Event {
public:
	static std::vector<std::map<std::string,int> > updateReqList;
	static void loadRequirements();
private:
	SDL_Surface *image;
	SDL_Surface *display;
	SDL_Surface *lcd;
	SDL_Surface *curser;
	SDL_Surface *timer;

	SDL_Surface *updateText;
	SDL_Surface *updateRequirements;

	SDL_Color green;

	SDL_Rect displayRect;
    SDL_Rect lcdRect;
	SDL_Rect imageRect;
	SDL_Rect curserRect;
	SDL_Rect timerRect;

	SDL_Rect updateTextRect;
	SDL_Rect updateRequirementsRect;

	Language lang;

	int level;
	unsigned currentItem;
	std::vector<items_t> itemlist;

	bool running;

	void init();
	void render();
	void build();
	void update();

	void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void onWiiButtonEvent(int button);

public:
	PDA(int level = 0);
	virtual ~PDA();

	int show();
	int getLevel() const;
};

#endif /* PDA_H_ */
