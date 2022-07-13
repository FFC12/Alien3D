#ifndef ALIEN3D_PHYSICS2D_H
#define ALIEN3D_PHYSICS2D_H

#include <box2d/box2d.h>
#include "Base.hpp"
#include "GameObject.h"
#include <engine/Sprite.h>

class PhysicsBody;

class PhysicsDebug : public b2Draw {
public:
    ~PhysicsDebug() {

    }

    void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override {
        // Standard OpenGL rendering stuff
//        glColor4f(color.r, color.g, color.b, color.a);
//        glBegin(GL_POLYGON);
//        for (int i = 0; i < vertexCount; i++) {
//            glVertex2f(vertices[i].x, vertices[i].y);
//        }
//        glEnd();
    }

    void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override {}

    void DrawCircle(const b2Vec2 &center, f32 radius, const b2Color &color) override {}

    void DrawSolidCircle(const b2Vec2 &center, f32 radius, const b2Vec2 &axis, const b2Color &color) override {}

    void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override {}

    void DrawTransform(const b2Transform &xf) override {}

    void DrawPoint(const b2Vec2 &p, float size, const b2Color &color) override {}
};

class WorldSimulation {
    friend class PhysicsBody;

public:
    static WorldSimulation &getInstance() {
        static WorldSimulation ins;
        return ins;
    }

    void step();

private:
    WorldSimulation();

    std::shared_ptr<b2World> m_World;
    PhysicsDebug m_DebugDraw;
};


#endif //ALIEN3D_PHYSICS2D_H

