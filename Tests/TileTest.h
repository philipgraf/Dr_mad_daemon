/*
 * TileTest.h
 *
 *  Created on: 14.12.2012
 *      Author: philip
 */

#ifndef TILETEST_H_
#define TILETEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include "../Tile.h"


class TileTest : public CppUnit::TestFixture {
private:
	CPPUNIT_TEST_SUITE( TileTest);

		CPPUNIT_TEST( constructorTest);

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void constructorTest();

private:
	Tile *testTile;
};

CPPUNIT_TEST_SUITE_REGISTRATION( TileTest);


#endif /* TILETEST_H_ */
