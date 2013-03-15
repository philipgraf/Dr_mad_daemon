#ifndef NOTIFICATION_H_
#define NOTIFICATION_H_

#define NOTIFICATION_INFO 0
#define NOTIFICATION_WARNING 1

#include <vector>
#include <string>
#include <SDL/SDL.h>

/** Notification class.
 * This class handles all notifications on the screen.
 * @author Philip Graf
 * @date 14.03.2013
 * @version 0.1.0 Alpha-State
 */
class Notification {
public:
	static std::vector<Notification*> notificationList; /**< list of all notifications */
private:
	SDL_Surface *notificationSurface; /**< the surface where the image and text will be drawn on */
	int counter; /**< visible counter if this is zero the notification will be destroyed */
public:
	Notification(std::string message, int displaySecs, int type = NOTIFICATION_INFO, std::string iconName = "");
	virtual ~Notification();

	void timeout();

	SDL_Surface* getNotificationSurface();
};

#endif /* NOTIFICATION_H_ */
