#include "Box2DDebugDraw.h"

#include "Game.h"

#include <vector>

Box2DDebugDraw::Box2DDebugDraw(float scaleFactor) : m_scaleFactor(scaleFactor) {}

void Box2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    std::vector<int16_t> vx;
    std::vector<int16_t> vy;
    for (int i = 0; i < vertexCount; ++i) {
        vx.push_back(vertices[i].x * m_scaleFactor);
        vy.push_back(vertices[i].y * m_scaleFactor);
    }
    Game::get().renderer().polygonRGBA(vx.data(), vy.data(), vertexCount, color.r * 255.0f, color.g * 255.0f,
                                       color.b * 255.0f, color.a * 255.0f);
}

void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    std::vector<int16_t> vx;
    std::vector<int16_t> vy;
    for (int i = 0; i < vertexCount; ++i) {
        vx.push_back(vertices[i].x * m_scaleFactor);
        vy.push_back(vertices[i].y * m_scaleFactor);
    }
    Game::get().renderer().filledPolygonRGBA(vx.data(), vy.data(), vertexCount, color.r * 255.0f, color.g * 255.0f,
                                             color.b * 255.0f, color.a * 255.0f);
}

void Box2DDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
    Game::get().renderer().circleRGBA(center.x * m_scaleFactor, center.y * m_scaleFactor, radius, color.r * 255.0f,
                                      color.g * 255.0f, color.b * 255.0f, color.a * 255.0f);
}

void Box2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
    Game::get().renderer().filledCircleRGBA(center.x * m_scaleFactor, center.y * m_scaleFactor, radius,
                                            color.r * 255.0f, color.g * 255.0f, color.b * 255.0f, color.a * 255.0f);
}

void Box2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
    Game::get().renderer().lineRGBA(p1.x * m_scaleFactor, p1.y * m_scaleFactor, p2.x * m_scaleFactor,
                                    p2.y * m_scaleFactor, color.r * 255.0f, color.g * 255.0f, color.b * 255.0f,
                                    color.a * 255.0f);
}

void Box2DDebugDraw::DrawTransform(const b2Transform& xf) {}

void Box2DDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) {
    Game::get().renderer().pixelRGBA(p.x * m_scaleFactor, p.y * m_scaleFactor, color.r * 255.0f, color.g * 255.0f,
                                     color.b * 255.0f, color.a * 255.0f);
}
