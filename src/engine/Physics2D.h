#ifndef ALIEN3D_PHYSICS2D_H
#define ALIEN3D_PHYSICS2D_H

#include <box2d/box2d.h>
#include "Base.hpp"
#include "GameObject.h"
#include <Application.h>
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

    //
    void step() {
        if (m_World->GetBodyCount() > 0) {
            for (i32 i = 0; i < 30; i++) {
                auto bodies = m_World->GetBodyList();

//                m_World->DebugDraw();
                auto body = bodies;
                while (body != nullptr) {
                    auto pos = body->GetPosition();
                    auto angle = body->GetAngle();

                    auto *sprite = reinterpret_cast<Sprite *>(body->GetUserData().pointer);
                    if (body->GetType() != b2_staticBody) {
                        sprite->m_Transformation->setPosition(Vector3(pos.x, pos.y, 0.0f));
//                        sprite->m_Transformation->positionLerp(Vector3(pos.x, pos.y, 0.0f), 100 * AlienApplication::DeltaTime);
                        sprite->m_Transformation->setRotation(angle);
                    }
                    body = body->GetNext();
                }

                m_World->Step(1.0f / 300.0f, 10, 20);
            }

            m_World->ClearForces();
        }
    }


private:
    WorldSimulation() {
        m_World = std::make_shared<b2World>(Gravity);
        m_World->SetDebugDraw(&m_DebugDraw);
        m_DebugDraw.SetFlags(b2Draw::e_shapeBit);
    }

    std::shared_ptr<b2World> m_World;
    PhysicsDebug m_DebugDraw;
    static inline b2Vec2 Gravity{0.0f, -10.0f};
};


#endif //ALIEN3D_PHYSICS2D_H

