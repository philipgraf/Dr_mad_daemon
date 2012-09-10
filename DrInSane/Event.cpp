/*
 * Event.cpp
 *
 *  Created on: 10.09.2012
 *      Author: philip
 */

#include "Event.h"

Event::Event() {

}

Event::~Event() {
}
/**
 * Funktion that execute the different Eventhandler
 *
 * @param event sdl event which contains happened UI interactions
 */
void Event::onEvent(SDL_Event *event) {
	switch (event->type) {
	case SDL_ACTIVEEVENT:
		switch (event->active.state) {
		case SDL_APPMOUSEFOCUS:
			if (event->active.gain)
				onMouseFocus();
			else
				onMouseBlur();
			break;
		case SDL_APPINPUTFOCUS:
			if (event->active.gain)
				onInputFocus();
			else
				onInputBlur();
			break;
		case SDL_APPACTIVE:
			if (event->active.gain)
				onRestore();
			else
				onMinimize();
			break;
		}
		break;
	case SDL_KEYDOWN:
		onKeyDown(event->key.keysym.sym, event->key.keysym.mod,
				event->key.keysym.unicode);
		break;
	case SDL_KEYUP:
		onKeyUP(event->key.keysym.sym, event->key.keysym.mod,
				event->key.keysym.unicode);
		break;
	case SDL_MOUSEMOTION:
		onMouseMove(event->motion.x, event->motion.y, event->motion.xrel,
				event->motion.yrel,
				(event->motion.state & SDL_BUTTON_LEFT) != 0,
				(event->motion.state & SDL_BUTTON_RIGHT) != 0,
				(event->motion.state & SDL_BUTTON_MIDDLE) != 0);
		break;

	case SDL_MOUSEBUTTONDOWN:
		switch (event->button.button) {
		case SDL_BUTTON_LEFT:
			onLButtonDown(event->button.x, event->button.y);
			break;
		case SDL_BUTTON_RIGHT:
			onRButtonDown(event->button.x, event->button.y);
			break;
		case SDL_BUTTON_MIDDLE:
			onMButtonDown(event->button.x, event->button.y);
			break;
		}
		break;
	case SDL_JOYAXISMOTION:
		onJoyAxis(event->jaxis.which, event->jaxis.axis, event->jaxis.value);
		break;
	case SDL_JOYBUTTONDOWN:
		onJoyButtonDown(event->jbutton.which, event->jbutton.button);
		break;
	case SDL_JOYBUTTONUP:
		onJoyButtonUp(event->jbutton.which, event->jbutton.button);
		break;
	case SDL_JOYHATMOTION:
		onJoyHat(event->jhat.which, event->jhat.value);
		break;
	case SDL_JOYBALLMOTION:
		onJoyBall(event->jball.which,event->jball.ball, event->jball.xrel, event->jball.yrel);
		break;
	case SDL_QUIT:
		onExit();
		break;
	case SDL_SYSWMEVENT:
		//ignore
		break;
	case SDL_VIDEOEXPOSE:
		onExpose();
		break;
	case SDL_VIDEORESIZE:
		onResize(event->resize.w, event->resize.h);
		break;
	default:
		onUser(event->user.type, event->user.code, event->user.data1, event->user.data2);
		break;
	}

}


void Event::onInputFocus() {
}
void Event::onInputBlur() {
}
void Event::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
}
void Event::onKeyUP(SDLKey sym, SDLMod mod, Uint16 unicode) {
}
void Event::onMouseFocus() {
}
void Event::onMouseBlur() {
}
void Event::onMouseMove(int mX, int mY, int xRel, int yRel, bool left,
		bool right, bool middle) {
}
void Event::onMouseWheel(bool up, bool down) {
}
void Event::onLButtonDown(int mX, int mY) {
}
void Event::onLButtonUp(int mX, int mY) {
}
void Event::onRButtonDown(int mX, int mY) {
}
void Event::onRButtonUp(int mX, int mY) {
}
void Event::onMButtonDown(int mX, int mY) {
}
void Event::onMButtonUp(int mX, int mY) {
}
void Event::onJoyAxis(Uint8 which, Uint8 axis, Sint16 value) {
}
void Event::onJoyButtonDown(Uint8 which, Uint8 button) {
}
void Event::onJoyButtonUp(Uint8 which, Uint8 button) {
}
void Event::onJoyHat(Uint8 which, Uint8 value) {
}
void Event::onJoyBall(Uint8 which, Uint8 ball, Sint16 xRel, Sint16 yRel) {
}
void Event::onMinimize() {
}
void Event::onRestore() {
}
void Event::onResize(int w, int h) {
}
void Event::onExpose() {
}
void Event::onExit() {
}
void Event::onUser(Uint8 type, int code, void *data1, void *data2) {
}
