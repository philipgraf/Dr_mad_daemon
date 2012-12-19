#include <cppunit/extensions/HelperMacros.h>
#include "../Tools.h"
#include <SDL/SDL.h>


class ToolsTest : public CppUnit::TestFixture {
private:
	CPPUNIT_TEST_SUITE( ToolsTest);

		CPPUNIT_TEST( loadImageTest);

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(){
		SDL_Init(SDL_INIT_EVERYTHING);
		screen = SDL_SetVideoMode(600,480,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
	}

	void loadImageTest(){
		SDL_Surface *s1;
		s1 = Tools::loadImage("../img/player.png");

		// should be able to load image
		CPPUNIT_ASSERT(s1!=NULL);
		SDL_FreeSurface(s1);

		// should fail TODO switch Tools::error to throw exception
		//s1 = Tools::loadImage("doesnotexist.jng");
		//CPPUNIT_ASSERT_EQUAL((SDL_Surface*)NULL,s1);
		//SDL_FreeSurface(s1);

	}

	void tearDown(){
		SDL_FreeSurface(screen);
		SDL_Quit();
	}
private:
	SDL_Surface *screen;
};

CPPUNIT_TEST_SUITE_REGISTRATION( ToolsTest);
