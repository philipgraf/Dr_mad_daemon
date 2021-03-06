/*
 * TextInput.cpp
 *
 *  Created on: 11.02.2013
 *      Author: philip
 */

#include "TextInput.h"

/**
 * Constructor of TextInput
 * This will generate a kind of input-"window" where text can be typed.
 *
 * @param title the text that will be shown in the titleSurface
 * @param maxSize the max count of digits that can be typed
 */
TextInput::TextInput(string title, int maxSize) {
	SDL_Surface *screen = SDL_GetVideoSurface();
	this->textInput = "";
	this->titleSurface = TTF_RenderUTF8_Solid(Game::curGame->getFont(), title.c_str(), (SDL_Color ) { 255, 255, 255 });
	this->maxSize = maxSize;
	this->background = SDL_CreateRGBSurface(SDL_HWSURFACE, 500, 150, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
	SDL_FillRect(background, &background->clip_rect, SDL_MapRGB(screen->format, 0, 0, 0));
	this->textInputBackground = SDL_CreateRGBSurface(SDL_HWSURFACE, background->w - 20, 55, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
	SDL_FillRect(textInputBackground, &textInputBackground->clip_rect, SDL_MapRGB(screen->format, 50, 50, 50));
	this->textInputSurface = TTF_RenderUTF8_Solid(Game::curGame->getFont(), textInput.c_str(), (SDL_Color ) { 255, 255, 255 });
}

/**
 * Destructor of TextInput.
 * This will free the used Surfaces
 */
TextInput::~TextInput() {
	SDL_FreeSurface(titleSurface);
	SDL_FreeSurface(background);
	SDL_FreeSurface(textInputBackground);
	SDL_FreeSurface(textInputSurface);

}
/**
 * Handles the typing.
 * At first the position of the surfaces will be calculated.
 * The Digits 0-9 and a-z will be added to textInput
 * BACKSPACE will delete the last digit and ESC will clear textInput and return "".
 * RETURN will end the typing and return textInput
 *
 * @see textInput
 */
string TextInput::getInput() {

	SDL_Surface *screen = SDL_GetVideoSurface();
	SDL_Event event;
	bool running = true;

	SDL_Rect backgroundDest;
	backgroundDest.x=screen->clip_rect.w / 2 - background->clip_rect.w / 2;
	backgroundDest.y=screen->h / 2 - background->clip_rect.h / 2;
	backgroundDest.w= background->clip_rect.w;
	backgroundDest.h= background->clip_rect.h;

	SDL_Rect textInputBackgroundDest;
	textInputBackgroundDest.x=10;
	textInputBackgroundDest.y=background->clip_rect.h - 65;
	textInputBackgroundDest.w=textInputBackground->clip_rect.w;
	textInputBackgroundDest.h=textInputBackground->clip_rect.h ;

	while (running) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			//TODO case sensitive
			case SDL_KEYDOWN:
				if ((event.key.keysym.sym >= '0' && event.key.keysym.sym <= '9') || (event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z')) {
					textInput += (char) event.key.keysym.sym;
				} else if (event.key.keysym.sym == SDLK_RETURN) {
					if (textInput != "") {
						running = false;
					}
				} else if (event.key.keysym.sym == SDLK_ESCAPE) {
					textInput = "";
					running = false;
				} else if (event.key.keysym.sym == SDLK_BACKSPACE) {
					if (textInput != "")
						textInput.erase(textInput.size() - 1);
				}
				break;
			}
		}
		SDL_FreeSurface(textInputSurface);
		this->textInputSurface = TTF_RenderUTF8_Solid(Game::curGame->getFont(), textInput.c_str(), (SDL_Color ) { 255, 255, 255 });

		SDL_BlitSurface(titleSurface, NULL, background, NULL);
		SDL_FillRect(textInputBackground, &textInputBackground->clip_rect, SDL_MapRGB(screen->format, 50, 50, 50));
		SDL_BlitSurface(textInputSurface, NULL, textInputBackground, NULL);
		SDL_BlitSurface(textInputBackground, NULL, background, &textInputBackgroundDest);
		SDL_BlitSurface(background, NULL, screen, &backgroundDest);

		SDL_Flip(screen);
	}

	return textInput;
}
