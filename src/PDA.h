#ifndef PDA_H_
#define PDA_H_

#define PDA_CLOCK 0
#define PDA_GLOVE 1

#include "Event.h"
#include "Language.h"

#include <SDL/SDL.h>
#include <vector>
#include <map>
#include <string>


typedef struct {
	SDL_Surface *itemname;
	SDL_Surface *amound;
} items_t;

/** PDA class.
 * This class handles the Player PDA
 * @author Philip Graf
 * @date 14.03.2013
 * @version 0.1.0 Alpha-State
 */
class PDA: public Event {
public:
	static std::vector<std::map<std::string,int> > updateReqList; /**< list of all level requirements */
	static void loadRequirements();
private:
	SDL_Surface *image; /**< the whole image of the PDA */
	SDL_Surface *display; /**< the main display of the PDA. the curser, timer and the items will be rendered on it */
	SDL_Surface *lcd; /**< the lcd of the PDA only available with level two or more */
	SDL_Surface *cursor; /**< the cursor of the PDA this is used to show the currentItem */
	SDL_Surface *timer; /**< the times left before the player dies */

	SDL_Surface *updateText; /**< the updatetext at the bottom of the PDA */
	SDL_Surface *updateRequirements; /**< shows the requirements for the next level  */

	SDL_Color green; /**< the color of the fonts */

	SDL_Rect displayRect; /**< the positions and the metrics of the display */
    SDL_Rect lcdRect;/**< the positions and the metrics of the lcd */
	SDL_Rect imageRect;/**< the positions and the metrics of the whole image  */
	SDL_Rect cursorRect;/**< the positions and the metrics of the cursor */
	SDL_Rect timerRect;/**< the positions and the metrics of the timer */

	SDL_Rect updateTextRect;/**< the positions and the metrics of the display */
	SDL_Rect updateRequirementsRect;/**< the positions and the metrics of the display */

	Language lang; /**< this objekt is used for translation*/

	int level; /**< the level of the PDA */
	unsigned currentItem; /**< the index of the current selected item */
	std::vector<items_t> itemlist; /**< contains all items of the player */

	bool running; /**< while this variable is true the PDA will be visible */

	void init();
	void render();
	void build();
	void update();

	void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void onWiiButtonEvent(int button);

public:
	PDA(int level = 0);
	virtual ~PDA();

	int show();
	int getLevel() const;
};

#endif /* PDA_H_ */
