#include "Notification.h"

#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "define.h"
#include "Game.h"

using namespace std;

vector<Notification*> Notification::notificationList;

/**
 * load the images and texts convert it to SDL_Surfaces and blit it on the notificationSurface.
 * @param message the message which will be shown
 * @param displaySecs the duration in seconds until the notification will be destroyed.
 * @param type the type of the Notification( INFO; WARRNING)
 * @param iconName the iconname without .bmp
 */
Notification::Notification(string message, int displaySecs, int type, string iconName) {
	SDL_Surface *temp;
	if ((temp = IMG_Load(NOTIFI"notification.png")) == NULL) {
		//TODO Throw exception
		std::cout << "unable to load notification.png" << std::endl;
	}
	notificationSurface = SDL_DisplayFormatAlpha(temp);
	SDL_FreeSurface(temp);

	counter = displaySecs * FPS;

	SDL_Color color;
	if (type == NOTIFICATION_INFO) {
		color.r = 255;
		color.g = 255;
		color.b = 255;
	} else if (type == NOTIFICATION_WARNING) {
		color.r = 255;
		color.g = 0;
		color.b = 0;
	}
	if (iconName == "") {
		if (type == NOTIFICATION_INFO) {
			iconName = NOTIFI"info";
		} else if (type == NOTIFICATION_WARNING) {
			iconName = NOTIFI"warning";
		}
	} else {
		iconName = ITEMS+iconName;
	}
	SDL_Surface *icon;

	if ((temp = SDL_LoadBMP((iconName+".bmp").c_str())) == NULL) {
		//TODO Throw exception
		std::cout << "unable to load: " << iconName+".bmp" << std::endl;
	} else {
		icon = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		if (temp != 0) {
			SDL_SetColorKey(icon, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(icon->format, 255, 0, 255));

		}
	}

	SDL_Rect dRect;
	dRect.x = 10;
	dRect.y = 6;
	dRect.w = icon->w;
	dRect.h = icon->h;

	SDL_BlitSurface(icon, NULL, notificationSurface, &dRect);

	SDL_Surface *text = TTF_RenderUTF8_Solid(Game::curGame->getFont(FONT_NOTIFICATION), message.c_str(), color);

	dRect.x = 60;
	dRect.y = 6;
	dRect.w = text->w;
	dRect.h = text->h;
	SDL_BlitSurface(text, NULL, notificationSurface, &dRect);

	notificationList.push_back(this);

}

/**
 * free all the allocated memory and remove it self from the notification list.
 */
Notification::~Notification() {
	SDL_FreeSurface(notificationSurface);
	std::vector<Notification*>::iterator pos;
	if ((pos = std::find(notificationList.begin(), notificationList.end(), this)) != notificationList.end()) {
		notificationList.erase(pos);
	}
}

/**
 * decrease the counter and commit suicide if it zero
 */
void Notification::timeout() {
	if (--counter <= 0) {
		//it's OK for an object to commit suicide
		delete this;
	}
}

/**
 * get the surface of the notification
 * @return the surface of the notification
 */
SDL_Surface* Notification::getNotificationSurface() {
	return notificationSurface;
}

