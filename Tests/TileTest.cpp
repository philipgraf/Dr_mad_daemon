/*
 * TileTest.cpp
 *
 *  Created on: 14.12.2012
 *      Author: philip
 */
#include <cppunit/extensions/HelperMacros.h>
#include "../Tile.h"


class TileTest : public CppUnit::TestFixture {
private:
	CPPUNIT_TEST_SUITE( TileTest);

		CPPUNIT_TEST( constructorTest);

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(){
		testTile = new Tile();
	}



	void constructorTest(){
		Tile t2(4);
		CPPUNIT_ASSERT_EQUAL(0,testTile->getId());
		CPPUNIT_ASSERT_EQUAL(4,t2.getId());
	}




	void tearDown(){
		delete testTile;
	}
private:
	Tile *testTile;
};

CPPUNIT_TEST_SUITE_REGISTRATION( TileTest);
