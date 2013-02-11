/*
 * TextInput.h
 *
 *  Created on: 11.02.2013
 *      Author: philip
 */

#ifndef TEXTINPUT_H_
#define TEXTINPUT_H_

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "Game.h"

using namespace std;

class TextInput {
private:
	string textInput;
	int maxSize;
	SDL_Surface *titleSurface;
	SDL_Surface *textInputSurface;
	SDL_Surface *background;
	SDL_Surface *textInputBackground;
public:
	TextInput(string title, int maxSize);
	virtual ~TextInput();
	string getInput();
};

#endif /* TEXTINPUT_H_ */
