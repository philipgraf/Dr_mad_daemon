/*
 * Event.cpp
 *
 *  Created on: 10.09.2012
 *      Author: philip
 */

#include "Event.h"
#include "define.h"

cwiid_mesg_callback_t wiimote_callback;

Event::Event() {
	wiimote = NULL;
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
		onKeyDown(event->key.keysym.sym, event->key.keysym.mod, event->key.keysym.unicode);
		break;
	case SDL_KEYUP:
		onKeyUP(event->key.keysym.sym, event->key.keysym.mod, event->key.keysym.unicode);
		break;
	case SDL_MOUSEMOTION:
		onMouseMove(event->motion.x, event->motion.y, event->motion.xrel, event->motion.yrel, (event->motion.state & SDL_BUTTON_LEFT) != 0, (event->motion.state & 4) != 0, (event->motion.state & SDL_BUTTON_MIDDLE) != 0);
		break;

	case SDL_MOUSEBUTTONDOWN:
		switch (event->button.button) {
		case SDL_BUTTON_LEFT:
			if (event->button.state == SDL_PRESSED)
				onLButtonDown(event->button.x, event->button.y);
			else if (event->button.state == SDL_RELEASED)
				onLButtonUp(event->button.x, event->button.y);
			break;
		case SDL_BUTTON_RIGHT:
			if (event->button.state == SDL_PRESSED)
				onRButtonDown(event->button.x, event->button.y);
			else if (event->button.state == SDL_RELEASED)
				onRButtonUp(event->button.x, event->button.y);
			break;
		case SDL_BUTTON_MIDDLE:
			if (event->button.state == SDL_PRESSED)
				onMButtonDown(event->button.x, event->button.y);
			else if (event->button.state == SDL_RELEASED)
				onMButtonUp(event->button.x, event->button.y);
			break;
		case SDL_BUTTON_WHEELDOWN:
			if (event->button.state == SDL_PRESSED)
				onMouseWheel(false, true);
			else if (event->button.state == SDL_RELEASED)
				onMouseWheel(false, false);
			break;
		case SDL_BUTTON_WHEELUP:
			if (event->button.state == SDL_PRESSED)
				onMouseWheel(true, true);
			else if (event->button.state == SDL_RELEASED)
				onMouseWheel(true, false);
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
		onJoyBall(event->jball.which, event->jball.ball, event->jball.xrel, event->jball.yrel);
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
	case SDL_USEREVENT:
		switch (event->user.code) {
		case MSGTYPE_BTN:
			onWiiButtonEvent(*((int*) (event->user.data1)));
			//delete (int*) event->user.data1;
			break;
		}
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
void Event::onMouseMove(int mX, int mY, int xRel, int yRel, bool left, bool right, bool middle) {
}
void Event::onMouseWheel(bool up, bool pressed) {
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

void Event::onWiiButtonEvent(int buttons) {
}

void Event::onUser(Uint8 type, int code, void *data1, void *data2) {
}

void Event::initWiimote() {
	blueaddr = (bdaddr_t ) { { 0, 0, 0, 0, 0, 0 } };
	SDL_Surface *wiimoteImage = SDL_LoadBMP(IMG"wiimote.bmp");
	SDL_SetColorKey(wiimoteImage, SDL_SRCCOLORKEY, SDL_MapRGB(wiimoteImage->format, 255, 0, 255));
	SDL_BlitSurface(wiimoteImage, NULL, SDL_GetVideoSurface(), NULL);
	SDL_Flip(SDL_GetVideoSurface());
	if ((wiimote = cwiid_connect(&blueaddr, CWIID_FLAG_MESG_IFC)) != NULL) {

		/**
		 * Set the callback function for the wiimote
		 * @param wiimote wiimote struct to identifiere whicht wiimote
		 * @param wiimote_callback pointer to the callbackfunction
		 */
		cwiid_set_mesg_callback(wiimote, wiimote_callback);

		/**
		 * Set the Reportmodes
		 * Available Modes:
		 * 	CWIID_RPT_STATUS     <- I think battery and extensions and so on
		 * 	CWIID_RPT_BTN		 <- enable buttons
		 * 	CWIID_RPT_ACC		 <- enable acceleration
		 * 	CWIID_RPT_IR		 <- enable infrared
		 * 	CWIID_RPT_NUNCHUK    <- enable nunchuk
		 * 	CWIID_RPT_CLASSIC    <- enable nintendo classic controller
		 */
		cwiid_command(wiimote, CWIID_CMD_RPT_MODE, CWIID_RPT_BTN);
	}
	SDL_FreeSurface(wiimoteImage);

}

void wiimote_callback(cwiid_wiimote_t *wiimote_my, int mesg_count, union cwiid_mesg mesg[], struct timespec *times) {

	for (int i = 0; i < mesg_count; i++) {
		switch (mesg[i].type) {
		case MSGTYPE_STATUS:

			break;
		case CWIID_MESG_BTN:
			SDL_Event event;
//			int *date1;
//			date1 = new int;
//			*date1 = ;
			event.type = SDL_USEREVENT;
			event.user.code = MSGTYPE_BTN;
			event.user.data1 = &mesg[i].btn_mesg.buttons;
			event.user.data2 = NULL;
			SDL_PushEvent(&event);
			break;
		case CWIID_MESG_ACC:
		case CWIID_MESG_IR:
		case CWIID_MESG_NUNCHUK:
		case CWIID_MESG_UNKNOWN:
			break;
		default:
			break;
		}
	}

}
