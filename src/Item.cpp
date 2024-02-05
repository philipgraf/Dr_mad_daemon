#include "Item.h"
#include "define.h"
#include "Game.h"

using namespace std;

vector<Item*> Item::itemlist;

/**
 * Load the image.
 * also a colorkey is set to make a defined color(0xFF00FF) transparent as .bmp do not provide an alpha channel but give the best performance
 * build the collision box of the item.
 * push the item in the itemList.
 *
 *
 * @param name the type of the item
 * @param x the x position of the item
 * @param y the y position of the item
 * @param relX the x value of the throw vector
 * @param relY the y value of the throw vector
 */
Item::Item(std::string name, int x, int y, int relX, int relY) {
	this->type = name;

	SDL_Surface *tmp = SDL_LoadBMP((ITEMS+name+".bmp").c_str());
	if (tmp != NULL) {
		image = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		if (image != 0) {
			SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(image->format, 255, 0, 255));

		}
	}

	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.fixedRotation = true;
	bodydef.position = b2Vec2(x, y);

	this->body = Game::curGame->getCurrentLevel()->getWorld()->CreateBody(&bodydef);

	b2PolygonShape polyshape;
	polyshape.SetAsBox(0.3125, 0.3125);

	b2FixtureDef *fixDef = new b2FixtureDef;
	fixDef->shape = &polyshape;
	fixDef->friction = 0.5;
	fixDef->restitution = 0.1;
	fixDef->density = 5;
	fixDef->filter.categoryBits = 1;
	fixDef->filter.maskBits =1;

	body->CreateFixture(fixDef);


	body->ApplyLinearImpulse(b2Vec2(relX, relY), body->GetWorldCenter(),true);

	timer = SDL_GetTicks();

	Item::itemlist.push_back(this);

}

/**
 * delete the item form the itemList and free all allocated memory
 */
Item::~Item() {
	SDL_FreeSurface(image);
	vector<Item*>::iterator pos;
	if ((pos = find(itemlist.begin(), itemlist.end(), this)) != itemlist.end()) {
		itemlist.erase(pos);
	}
	Game::curGame->getCurrentLevel()->getWorld()->DestroyBody(body);
}

/**
 * enable collision with the player after 1000 milliseconds and check the collision.
 * If a collision is detected the item will be added to the colliding entity.
 */
void Item::logic() {
	if(SDL_GetTicks() - timer > 1000){
		body->GetFixtureList()->SetFilterData(b2Filter());
	}
	for (b2ContactEdge *contactEdge = body->GetContactList(); contactEdge; contactEdge = contactEdge->next) {
		if (contactEdge->contact->GetFixtureA()->GetBody()->GetUserData().pointer != NULL) {
			((Entity*) contactEdge->contact->GetFixtureA()->GetBody()->GetUserData().pointer)->addItem(type);
			//it's OK for an object to commit suicide
			delete this;
			break;
		} else if (contactEdge->contact->GetFixtureB()->GetBody()->GetUserData().pointer != NULL) {
			((Entity*) contactEdge->contact->GetFixtureB()->GetBody()->GetUserData().pointer)->addItem(type);
			//it's OK for an object to commit suicide
			delete this;
			break;
		}
	}
}

/**
 * get the image of the Item
 * @return the image of the Item
 */
SDL_Surface *Item::getImage() {
	return image;
}

/**
 * get the x,y and the width and height of the item and build it as SDL_Rect.
 * @return the proportion of the item
 */
SDL_Rect Item::getClipRect() {
	SDL_Rect retRect;
	retRect.w = image->clip_rect.w;
	retRect.h = image->clip_rect.h;
	retRect.x = body->GetPosition().x * TILESIZE - retRect.w / 2;
	retRect.y = body->GetPosition().y * TILESIZE - retRect.h / 2;
	return retRect;
}
