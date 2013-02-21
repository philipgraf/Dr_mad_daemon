/*
 * Event.h
 *
 *  Created on: 10.09.2012
 *      Author: philip
 */
#ifndef EVENT_H_
#define EVENT_H_

/*********************************** WII DEFINES ****************************************************************************/

/* Message Types and SDL_UserEvent Codes */
#define MSGTYPE_STATUS 0
#define MSGTYPE_BTN 1
#define MSGTYPE_ACC 2
#define MSGTYPE_IR 3
#define MSGTYPE_NUNCHUK 4

/* Wii Buttons */
#define WII_BTN_2 		0x0001
#define WII_BTN_1 		0x0002
#define WII_BTN_B 		0x0004
#define WII_BTN_A 		0x0008
#define WII_BTN_MINUS 	0x0010
#define WII_BTN_HOME	0x0080
#define WII_BTN_LEFT	0x0100
#define WII_BTN_RIGHT	0x0200
#define WII_BTN_DOWN	0x0400
#define WII_BTN_UP		0x0800
#define WII_BTN_PLUS	0x1000

/****************************************************************************************************************************/

#include <SDL/SDL.h>
#include <cwiid.h>

/**
 * This Class handles all Input Events
 * Maybe wii and SNES too
 */
class Event {
private:
	bdaddr_t blueaddr;
	cwiid_wiimote_t *wiimote;
public:
	Event();
	virtual ~Event();

	void initWiimote();

	virtual void onEvent(SDL_Event *event);
	virtual void onInputFocus();
	virtual void onInputBlur();
	virtual void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void onKeyUP(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void onMouseFocus();
	virtual void onMouseBlur();
	virtual void onMouseMove(int mX, int mY, int xRel, int yRel, bool left, bool right, bool middle);
	virtual void onMouseWheel(bool up, bool down);
	virtual void onLButtonDown(int mX, int mY);
	virtual void onLButtonUp(int mX, int mY);
	virtual void onRButtonDown(int mX, int mY);
	virtual void onRButtonUp(int mX, int mY);
	virtual void onMButtonDown(int mX, int mY);
	virtual void onMButtonUp(int mX, int mY);
	virtual void onJoyAxis(Uint8 which, Uint8 axis, Sint16 value);
	virtual void onJoyButtonDown(Uint8 which, Uint8 button);
	virtual void onJoyButtonUp(Uint8 which, Uint8 button);
	virtual void onJoyHat(Uint8 which, Uint8 value);
	virtual void onJoyBall(Uint8 which, Uint8 ball, Sint16 xRel, Sint16 yRel);
	virtual void onMinimize();
	virtual void onRestore();
	virtual void onResize(int w, int h);
	virtual void onExpose();
	virtual void onExit();
	virtual void onWiiButtonEvent(int buttons);
	virtual void onUser(Uint8 type, int code, void *data1, void *data2);
};

#endif /* EVENT_H_ */
