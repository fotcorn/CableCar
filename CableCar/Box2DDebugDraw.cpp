#include "Box2DDebugDraw.h"

#include "SDL2_gfxPrimitives.h"

#include <vector>

Box2DDebugDraw::Box2DDebugDraw(SDL_Renderer* renderer, float scaleFactor)
    : m_renderer(renderer), m_scaleFactor(scaleFactor) {}

void Box2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    std::vector<Sint16> vx;
    std::vector<Sint16> vy;
    for (int i = 0; i < vertexCount; ++i) {
        vx.push_back(vertices[i].x * m_scaleFactor);
        vy.push_back(vertices[i].y * m_scaleFactor);
    }
    polygonRGBA(m_renderer, vx.data(), vy.data(), vertexCount, color.r * 255.0f, color.g * 255.0f, color.b * 255.0f,
                color.a * 255.0f);
}

void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    std::vector<Sint16> vx;
    std::vector<Sint16> vy;
    for (int i = 0; i < vertexCount; ++i) {
        vx.push_back(vertices[i].x * m_scaleFactor);
        vy.push_back(vertices[i].y * m_scaleFactor);
    }
    filledPolygonRGBA(m_renderer, vx.data(), vy.data(), vertexCount, color.r * 255.0f, color.g * 255.0f,
                      color.b * 255.0f, color.a * 255.0f);
}

void Box2DDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
    circleRGBA(m_renderer, center.x * m_scaleFactor, center.y * m_scaleFactor, radius, color.r * 255.0f,
               color.g * 255.0f, color.b * 255.0f, color.a * 255.0f);
}

void Box2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
    filledCircleRGBA(m_renderer, center.x * m_scaleFactor, center.y * m_scaleFactor, radius, color.r * 255.0f,
                     color.g * 255.0f, color.b * 255.0f, color.a * 255.0f);
}

void Box2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
    lineRGBA(m_renderer, p1.x * m_scaleFactor, p1.y * m_scaleFactor, p2.x * m_scaleFactor, p2.y * m_scaleFactor,
             color.r * 255.0f, color.g * 255.0f, color.b * 255.0f, color.a * 255.0f);
}

void Box2DDebugDraw::DrawTransform(const b2Transform& xf) {}

void Box2DDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) {
    pixelRGBA(m_renderer, p.x * m_scaleFactor, p.y * m_scaleFactor, color.r * 255.0f, color.g * 255.0f,
              color.b * 255.0f, color.a * 255.0f);
}
