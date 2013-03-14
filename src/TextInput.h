#ifndef TEXTINPUT_H_
#define TEXTINPUT_H_

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "Game.h"

using namespace std;

/** TextInput class.
 * This class handles "textinputfields";
 * @author Philip Graf
 * @date 14.03.2013
 * @version 0.1.0 Alpha-State
 */

class TextInput {
private:
	string textInput; /**< Stores the typed text*/
	int maxSize; /**< Defines how many digits can be typed*/
	SDL_Surface *titleSurface; /**< Surface above the input where the title will be displayed*/
	SDL_Surface *textInputSurface; /**< On this surface the typed text will be shown (the font)*/
	SDL_Surface *background; /**< Simply a background for the input-"window"*/
	SDL_Surface *textInputBackground; /**< The background for the textInputSurface*/
public:
	TextInput(string title, int maxSize);
	virtual ~TextInput();
	string getInput();
};

#endif
