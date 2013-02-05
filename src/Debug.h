/*
 * Debug.h
 *
 *  Created on: 31.01.2013
 *      Author: philip
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <Box2D/Box2D.h>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <iostream>
#include "define.h"

class Debug: public b2Draw {
private:
	int fps;
public:
	Debug();
	virtual ~Debug();
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount,
			const b2Color& color);
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount,
			const b2Color& color);
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	void DrawSolidCircle(const b2Vec2& center, float32 radius,
			const b2Vec2& axis, const b2Color& color);
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	void DrawTransform(const b2Transform& xf);
};

#endif /* DEBUG_H_ */