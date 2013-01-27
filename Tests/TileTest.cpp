/*
 * TileTest.cpp
 *
 *  Created on: 14.12.2012
 *      Author: philip
 */
#include <cppunit/extensions/HelperMacros.h>
#include "../src/Tile.h"


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
		Tile t3(65536);
		Tile t4(589827);

		CPPUNIT_ASSERT_EQUAL((u_int16_t)0,testTile->getId());
		CPPUNIT_ASSERT_EQUAL((u_int16_t)4,t2.getId());
		CPPUNIT_ASSERT_EQUAL((u_int16_t)0,t3.getId());
		CPPUNIT_ASSERT_EQUAL((u_int16_t)3,t4.getId());

		CPPUNIT_ASSERT_EQUAL((u_int64_t)0,testTile->getFlags());
		CPPUNIT_ASSERT_EQUAL((u_int64_t)0,t2.getFlags());
		CPPUNIT_ASSERT_EQUAL((u_int64_t)1,t3.getFlags());
		CPPUNIT_ASSERT_EQUAL((u_int64_t)9,t4.getFlags());
	}




	void tearDown(){
		delete testTile;
	}
private:
	Tile *testTile;
};

CPPUNIT_TEST_SUITE_REGISTRATION( TileTest);
