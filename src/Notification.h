/*
 * Notification.h
 *
 *  Created on: 19.02.2013
 *      Author: philip
 */

#ifndef NOTIFICATION_H_
#define NOTIFICATION_H_

#define NOTIFICATION_INFO 0
#define NOTIFICATION_WARNING 1


#include <vector>
#include <string>
#include <SDL/SDL.h>

class Notification {
public:
	static std::vector<Notification*> notificationList;
private:
	SDL_Surface *notificationSurface;
	int counter;
public:
	Notification(std::string message,int displaySecs ,int type=NOTIFICATION_INFO, std::string iconName="");
	virtual ~Notification();

	void timeout();

	SDL_Surface* getNotificationSurface();
};

#endif /* NOTIFICATION_H_ */
