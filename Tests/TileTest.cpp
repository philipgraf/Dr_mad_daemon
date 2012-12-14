/*
 * TileTest.cpp
 *
 *  Created on: 14.12.2012
 *      Author: philip
 */

#include "TileTest.h"



void TileTest::setUp() {
	testTile = new Tile();
}

void TileTest::tearDown() {
	delete testTile;
}

void TileTest::constructorTest() {
	CPPUNIT_ASSERT_EQUAL(0,testTile->getId());
}
