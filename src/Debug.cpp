#include "Debug.h"
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "define.h"

#define SCALE 4

using namespace std;

void Debug::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	Sint16 *x = new Sint16[vertexCount];
	Sint16 *y = new Sint16[vertexCount];
	for (int i = 0; i < vertexCount; i++) {
		x[i] = vertices[i].x * TILESIZE / SCALE;
		y[i] = vertices[i].y * TILESIZE / SCALE;
	}
	polygonRGBA(SDL_GetVideoSurface(), x, y, vertexCount, 255, 255, 255, 255);
}

void Debug::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {

	Sint16 *x = new Sint16[vertexCount];
	Sint16 *y = new Sint16[vertexCount];
	for (int i = 0; i < vertexCount; i++) {
		x[i] = vertices[i].x * TILESIZE / SCALE;
		y[i] = vertices[i].y * TILESIZE / SCALE;
	}
	filledPolygonRGBA(SDL_GetVideoSurface(), x, y, vertexCount, color.r, color.g, color.b, 127);
}

void Debug::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
	circleRGBA(SDL_GetVideoSurface(), center.x * TILESIZE / SCALE, center.y * TILESIZE / SCALE, radius * TILESIZE / SCALE, color.r, color.g, color.b, 127);
}

void Debug::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
	filledCircleRGBA(SDL_GetVideoSurface(), center.x * TILESIZE / SCALE, center.y * TILESIZE / SCALE, radius * TILESIZE / SCALE, color.r, color.g, color.b, 127);
	b2Vec2 p = center + radius * axis;
	lineRGBA(SDL_GetVideoSurface(), center.x * TILESIZE / SCALE, center.y * TILESIZE / SCALE, p.x * TILESIZE / SCALE, p.y * TILESIZE / SCALE, color.r, color.g, color.b, 127);
}

void Debug::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
	lineRGBA(SDL_GetVideoSurface(), p1.x * TILESIZE / SCALE, p1.y * TILESIZE / SCALE, p2.x * TILESIZE / SCALE, p2.y * TILESIZE / SCALE, color.r, color.g, color.b, 255);

}

void Debug::DrawTransform(const b2Transform& xf) {
}
