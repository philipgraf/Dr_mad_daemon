/*
 * Event.h
 *
 *  Created on: 10.09.2012
 *      Author: philip
 */

#include "includes.h"

#ifndef EVENT_H_
#define EVENT_H_


class Event {
public:
	Event();
	virtual ~Event();

	virtual void onEvent(SDL_Event *event);
	virtual void onInputFocus();
	virtual void onInputBlur();
	virtual void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void onKeyUP(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void onMouseFocus();
	virtual void onMouseBlur();
	virtual void onMouseMove(int mX, int mY, int xRel, int yRel, bool left, bool right, bool middle);
	virtual void onMouseWheel(bool up, bool down);
	virtual void onLButtonDown(int mX,int mY);
	virtual void onLButtonUp(int mX,int mY);
	virtual void onRButtonDown(int mX,int mY);
	virtual void onRButtonUp(int mX,int mY);
	virtual void onMButtonDown(int mX,int mY);
	virtual void onMButtonUp(int mX,int mY);
	virtual void onJoyAxis(Uint8 which, Uint8 axis,Sint16 value);
	virtual void onJoyButtonDown(Uint8 which, Uint8 button);
	virtual void onJoyButtonUp(Uint8 which, Uint8 button);
	virtual void onJoyHat(Uint8 which, Uint8 value);
	virtual void onJoyBall(Uint8 which, Uint8 ball, Sint16 xRel, Sint16 yRel);
	virtual void onMinimize();
	virtual void onRestore();
	virtual void onResize(int w, int h);
	virtual void onExpose();
	virtual void onExit();
	virtual void onUser(Uint8 type, int code, void *data1, void *data2);
};

#endif /* EVENT_H_ */
